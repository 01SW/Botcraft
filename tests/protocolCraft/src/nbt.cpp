#include <catch2/catch_test_macros.hpp>

#include "protocolCraft/Types/NBT/NBT.hpp"

using namespace ProtocolCraft;

TEST_CASE("Empty NBT")
{
    NBT::Value n;
    std::vector<unsigned char> data = { 0x00 };
    ReadIterator iter = data.begin();
    size_t length = data.size();

    SECTION("Empty")
    {
        CHECK_FALSE(n.HasData());
    }

    SECTION("Read Empty")
    {
        n = ReadData<NBT::Value>(iter, length);
        CHECK_FALSE(n.HasData());
        CHECK_THROWS(n["a"]);
    }

    SECTION("Serialization")
    {
        std::vector<unsigned char> serialized;
        serialized.reserve(data.size());

        WriteData<NBT::Value>(n, serialized);

        CHECK(serialized == data);
    }
}

TEST_CASE("Tag short only")
{
    std::vector<unsigned char> data = { 0x02, 0x00, 0x09, 0x73,0x68,0x6F, 0x72, 0x74, 0x54, 0x65, 0x73, 0x74, 0x7F, 0xFF };
    ReadIterator iter = data.begin();
    size_t length = data.size();

    SECTION("Not a valid NBT")
    {
        NBT::Value n;
        CHECK_THROWS(n.Read(iter, length));
    }

    SECTION("Valid short NamedTag")
    {
        NBT::Tag t = ReadData<NBT::Tag>(iter, length);
        CHECK(t.GetName() == "shortTest");
        CHECK(t.is<short>());
        CHECK(t.get<NBT::TagShort>() == 32767);
        CHECK(t.get<short>() == 32767);
        CHECK_THROWS(t.get<NBT::TagCompound>());
        CHECK_THROWS(t.get<NBT::TagList>());
        CHECK_THROWS(t.get<NBT::TagInt>());
        CHECK_THROWS(t["A"]);
        CHECK_THROWS(t.size());
        CHECK_THROWS(t.as_list_of<short>());
        CHECK_THROWS(t.as_list_of<NBT::TagCompound>());

        Json::Value check_serialization = { { "name", "shortTest"}, {"type", "TagShort"}, {"content", 32767} };
        CHECK(t.Serialize().Dump() == check_serialization.Dump());
    }
}

TEST_CASE("test nbt")
{
    std::vector<unsigned char> data = { 
        0x0A, // TagCompound
        0x00, 0x0B, // Name length
        0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, // Name
        0x08, // TagString
        0x00, 0x04, // Name length
        0x6E, 0x61, 0x6D, 0x65, // Name
        0x00, 0x09, // String length
        0x42, 0x61, 0x6E, 0x61, 0x6E, 0x72, 0x61, 0x6D, 0x61, // String content
        0x00 // TagEnd
    };
    ReadIterator iter = data.begin();
    size_t length = data.size();

    NBT::Value nbt = ReadData<NBT::Value>(iter, length);

    CHECK(nbt.HasData());
    CHECK(nbt.is<NBT::TagCompound>());
    CHECK(nbt.size() == 1);
    CHECK(nbt.GetName() == "hello world");
    CHECK(nbt["name"].is<NBT::TagString>());
    CHECK(nbt["name"].get<NBT::TagString>() == "Bananrama");

    std::vector<unsigned char> serialized;
    serialized.reserve(data.size());

    WriteData<NBT::Value>(nbt, serialized);

    CHECK(serialized == data);
}

TEST_CASE("Uncompressed bigtest nbt")
{
    std::vector<unsigned char> data = {
        0x0A, 0x00, 0x05, 0x4C, 0x65, 0x76, 0x65, 0x6C, 0x04, 0x00, 0x08, 0x6C, 0x6F, 0x6E, 0x67, 0x54, 0x65, 0x73, 0x74, 0x7F, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0x00, 0x09, 0x73, 0x68, 0x6F, 0x72, 0x74, 0x54, 0x65, 0x73, 0x74, 0x7F, 0xFF, 0x08,
        0x00, 0x0A, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x54, 0x65, 0x73, 0x74, 0x00, 0x29, 0x48, 0x45, 0x4C, 0x4C, 0x4F, 0x20, 0x57,
        0x4F, 0x52, 0x4C, 0x44, 0x20, 0x54, 0x48, 0x49, 0x53, 0x20, 0x49, 0x53, 0x20, 0x41, 0x20, 0x54, 0x45, 0x53, 0x54, 0x20, 0x53,
        0x54, 0x52, 0x49, 0x4E, 0x47, 0x20, 0xC3, 0x85, 0xC3, 0x84, 0xC3, 0x96, 0x21, 0x05, 0x00, 0x09, 0x66, 0x6C, 0x6F, 0x61, 0x74,
        0x54, 0x65, 0x73, 0x74, 0x3E, 0xFF, 0x18, 0x32, 0x03, 0x00, 0x07, 0x69, 0x6E, 0x74, 0x54, 0x65, 0x73, 0x74, 0x7F, 0xFF, 0xFF,
        0xFF, 0x0A, 0x00, 0x14, 0x6E, 0x65, 0x73, 0x74, 0x65, 0x64, 0x20, 0x63, 0x6F, 0x6D, 0x70, 0x6F, 0x75, 0x6E, 0x64, 0x20, 0x74,
        0x65, 0x73, 0x74, 0x0A, 0x00, 0x03, 0x68, 0x61, 0x6D, 0x08, 0x00, 0x04, 0x6E, 0x61, 0x6D, 0x65, 0x00, 0x06, 0x48, 0x61, 0x6D,
        0x70, 0x75, 0x73, 0x05, 0x00, 0x05, 0x76, 0x61, 0x6C, 0x75, 0x65, 0x3F, 0x40, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x03, 0x65, 0x67,
        0x67, 0x08, 0x00, 0x04, 0x6E, 0x61, 0x6D, 0x65, 0x00, 0x07, 0x45, 0x67, 0x67, 0x62, 0x65, 0x72, 0x74, 0x05, 0x00, 0x05, 0x76,
        0x61, 0x6C, 0x75, 0x65, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x0F, 0x6C, 0x69, 0x73, 0x74, 0x54, 0x65, 0x73, 0x74,
        0x20, 0x28, 0x6C, 0x6F, 0x6E, 0x67, 0x29, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x09, 0x00, 0x13, 0x6C, 0x69, 0x73, 0x74, 0x54, 0x65, 0x73, 0x74,
        0x20, 0x28, 0x63, 0x6F, 0x6D, 0x70, 0x6F, 0x75, 0x6E, 0x64, 0x29, 0x0A, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x04, 0x6E, 0x61,
        0x6D, 0x65, 0x00, 0x0F, 0x43, 0x6F, 0x6D, 0x70, 0x6F, 0x75, 0x6E, 0x64, 0x20, 0x74, 0x61, 0x67, 0x20, 0x23, 0x30, 0x04, 0x00,
        0x0A, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x2D, 0x6F, 0x6E, 0x00, 0x00, 0x01, 0x26, 0x52, 0x37, 0xD5, 0x8D, 0x00, 0x08,
        0x00, 0x04, 0x6E, 0x61, 0x6D, 0x65, 0x00, 0x0F, 0x43, 0x6F, 0x6D, 0x70, 0x6F, 0x75, 0x6E, 0x64, 0x20, 0x74, 0x61, 0x67, 0x20,
        0x23, 0x31, 0x04, 0x00, 0x0A, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x2D, 0x6F, 0x6E, 0x00, 0x00, 0x01, 0x26, 0x52, 0x37,
        0xD5, 0x8D, 0x00, 0x01, 0x00, 0x08, 0x62, 0x79, 0x74, 0x65, 0x54, 0x65, 0x73, 0x74, 0x7F, 0x07, 0x00, 0x65, 0x62, 0x79, 0x74,
        0x65, 0x41, 0x72, 0x72, 0x61, 0x79, 0x54, 0x65, 0x73, 0x74, 0x20, 0x28, 0x74, 0x68, 0x65, 0x20, 0x66, 0x69, 0x72, 0x73, 0x74,
        0x20, 0x31, 0x30, 0x30, 0x30, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65, 0x73, 0x20, 0x6F, 0x66, 0x20, 0x28, 0x6E, 0x2A, 0x6E, 0x2A,
        0x32, 0x35, 0x35, 0x2B, 0x6E, 0x2A, 0x37, 0x29, 0x25, 0x31, 0x30, 0x30, 0x2C, 0x20, 0x73, 0x74, 0x61, 0x72, 0x74, 0x69, 0x6E,
        0x67, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x6E, 0x3D, 0x30, 0x20, 0x28, 0x30, 0x2C, 0x20, 0x36, 0x32, 0x2C, 0x20, 0x33, 0x34,
        0x2C, 0x20, 0x31, 0x36, 0x2C, 0x20, 0x38, 0x2C, 0x20, 0x2E, 0x2E, 0x2E, 0x29, 0x29, 0x00, 0x00, 0x03, 0xE8, 0x00, 0x3E, 0x22,
        0x10, 0x08, 0x0A, 0x16, 0x2C, 0x4C, 0x12, 0x46, 0x20, 0x04, 0x56, 0x4E, 0x50, 0x5C, 0x0E, 0x2E, 0x58, 0x28, 0x02, 0x4A, 0x38,
        0x30, 0x32, 0x3E, 0x54, 0x10, 0x3A, 0x0A, 0x48, 0x2C, 0x1A, 0x12, 0x14, 0x20, 0x36, 0x56, 0x1C, 0x50, 0x2A, 0x0E, 0x60, 0x58,
        0x5A, 0x02, 0x18, 0x38, 0x62, 0x32, 0x0C, 0x54, 0x42, 0x3A, 0x3C, 0x48, 0x5E, 0x1A, 0x44, 0x14, 0x52, 0x36, 0x24, 0x1C, 0x1E,
        0x2A, 0x40, 0x60, 0x26, 0x5A, 0x34, 0x18, 0x06, 0x62, 0x00, 0x0C, 0x22, 0x42, 0x08, 0x3C, 0x16, 0x5E, 0x4C, 0x44, 0x46, 0x52,
        0x04, 0x24, 0x4E, 0x1E, 0x5C, 0x40, 0x2E, 0x26, 0x28, 0x34, 0x4A, 0x06, 0x30, 0x00, 0x3E, 0x22, 0x10, 0x08, 0x0A, 0x16, 0x2C,
        0x4C, 0x12, 0x46, 0x20, 0x04, 0x56, 0x4E, 0x50, 0x5C, 0x0E, 0x2E, 0x58, 0x28, 0x02, 0x4A, 0x38, 0x30, 0x32, 0x3E, 0x54, 0x10,
        0x3A, 0x0A, 0x48, 0x2C, 0x1A, 0x12, 0x14, 0x20, 0x36, 0x56, 0x1C, 0x50, 0x2A, 0x0E, 0x60, 0x58, 0x5A, 0x02, 0x18, 0x38, 0x62,
        0x32, 0x0C, 0x54, 0x42, 0x3A, 0x3C, 0x48, 0x5E, 0x1A, 0x44, 0x14, 0x52, 0x36, 0x24, 0x1C, 0x1E, 0x2A, 0x40, 0x60, 0x26, 0x5A,
        0x34, 0x18, 0x06, 0x62, 0x00, 0x0C, 0x22, 0x42, 0x08, 0x3C, 0x16, 0x5E, 0x4C, 0x44, 0x46, 0x52, 0x04, 0x24, 0x4E, 0x1E, 0x5C,
        0x40, 0x2E, 0x26, 0x28, 0x34, 0x4A, 0x06, 0x30, 0x00, 0x3E, 0x22, 0x10, 0x08, 0x0A, 0x16, 0x2C, 0x4C, 0x12, 0x46, 0x20, 0x04,
        0x56, 0x4E, 0x50, 0x5C, 0x0E, 0x2E, 0x58, 0x28, 0x02, 0x4A, 0x38, 0x30, 0x32, 0x3E, 0x54, 0x10, 0x3A, 0x0A, 0x48, 0x2C, 0x1A,
        0x12, 0x14, 0x20, 0x36, 0x56, 0x1C, 0x50, 0x2A, 0x0E, 0x60, 0x58, 0x5A, 0x02, 0x18, 0x38, 0x62, 0x32, 0x0C, 0x54, 0x42, 0x3A,
        0x3C, 0x48, 0x5E, 0x1A, 0x44, 0x14, 0x52, 0x36, 0x24, 0x1C, 0x1E, 0x2A, 0x40, 0x60, 0x26, 0x5A, 0x34, 0x18, 0x06, 0x62, 0x00,
        0x0C, 0x22, 0x42, 0x08, 0x3C, 0x16, 0x5E, 0x4C, 0x44, 0x46, 0x52, 0x04, 0x24, 0x4E, 0x1E, 0x5C, 0x40, 0x2E, 0x26, 0x28, 0x34,
        0x4A, 0x06, 0x30, 0x00, 0x3E, 0x22, 0x10, 0x08, 0x0A, 0x16, 0x2C, 0x4C, 0x12, 0x46, 0x20, 0x04, 0x56, 0x4E, 0x50, 0x5C, 0x0E,
        0x2E, 0x58, 0x28, 0x02, 0x4A, 0x38, 0x30, 0x32, 0x3E, 0x54, 0x10, 0x3A, 0x0A, 0x48, 0x2C, 0x1A, 0x12, 0x14, 0x20, 0x36, 0x56,
        0x1C, 0x50, 0x2A, 0x0E, 0x60, 0x58, 0x5A, 0x02, 0x18, 0x38, 0x62, 0x32, 0x0C, 0x54, 0x42, 0x3A, 0x3C, 0x48, 0x5E, 0x1A, 0x44,
        0x14, 0x52, 0x36, 0x24, 0x1C, 0x1E, 0x2A, 0x40, 0x60, 0x26, 0x5A, 0x34, 0x18, 0x06, 0x62, 0x00, 0x0C, 0x22, 0x42, 0x08, 0x3C,
        0x16, 0x5E, 0x4C, 0x44, 0x46, 0x52, 0x04, 0x24, 0x4E, 0x1E, 0x5C, 0x40, 0x2E, 0x26, 0x28, 0x34, 0x4A, 0x06, 0x30, 0x00, 0x3E,
        0x22, 0x10, 0x08, 0x0A, 0x16, 0x2C, 0x4C, 0x12, 0x46, 0x20, 0x04, 0x56, 0x4E, 0x50, 0x5C, 0x0E, 0x2E, 0x58, 0x28, 0x02, 0x4A,
        0x38, 0x30, 0x32, 0x3E, 0x54, 0x10, 0x3A, 0x0A, 0x48, 0x2C, 0x1A, 0x12, 0x14, 0x20, 0x36, 0x56, 0x1C, 0x50, 0x2A, 0x0E, 0x60,
        0x58, 0x5A, 0x02, 0x18, 0x38, 0x62, 0x32, 0x0C, 0x54, 0x42, 0x3A, 0x3C, 0x48, 0x5E, 0x1A, 0x44, 0x14, 0x52, 0x36, 0x24, 0x1C,
        0x1E, 0x2A, 0x40, 0x60, 0x26, 0x5A, 0x34, 0x18, 0x06, 0x62, 0x00, 0x0C, 0x22, 0x42, 0x08, 0x3C, 0x16, 0x5E, 0x4C, 0x44, 0x46,
        0x52, 0x04, 0x24, 0x4E, 0x1E, 0x5C, 0x40, 0x2E, 0x26, 0x28, 0x34, 0x4A, 0x06, 0x30, 0x00, 0x3E, 0x22, 0x10, 0x08, 0x0A, 0x16,
        0x2C, 0x4C, 0x12, 0x46, 0x20, 0x04, 0x56, 0x4E, 0x50, 0x5C, 0x0E, 0x2E, 0x58, 0x28, 0x02, 0x4A, 0x38, 0x30, 0x32, 0x3E, 0x54,
        0x10, 0x3A, 0x0A, 0x48, 0x2C, 0x1A, 0x12, 0x14, 0x20, 0x36, 0x56, 0x1C, 0x50, 0x2A, 0x0E, 0x60, 0x58, 0x5A, 0x02, 0x18, 0x38,
        0x62, 0x32, 0x0C, 0x54, 0x42, 0x3A, 0x3C, 0x48, 0x5E, 0x1A, 0x44, 0x14, 0x52, 0x36, 0x24, 0x1C, 0x1E, 0x2A, 0x40, 0x60, 0x26,
        0x5A, 0x34, 0x18, 0x06, 0x62, 0x00, 0x0C, 0x22, 0x42, 0x08, 0x3C, 0x16, 0x5E, 0x4C, 0x44, 0x46, 0x52, 0x04, 0x24, 0x4E, 0x1E,
        0x5C, 0x40, 0x2E, 0x26, 0x28, 0x34, 0x4A, 0x06, 0x30, 0x00, 0x3E, 0x22, 0x10, 0x08, 0x0A, 0x16, 0x2C, 0x4C, 0x12, 0x46, 0x20,
        0x04, 0x56, 0x4E, 0x50, 0x5C, 0x0E, 0x2E, 0x58, 0x28, 0x02, 0x4A, 0x38, 0x30, 0x32, 0x3E, 0x54, 0x10, 0x3A, 0x0A, 0x48, 0x2C,
        0x1A, 0x12, 0x14, 0x20, 0x36, 0x56, 0x1C, 0x50, 0x2A, 0x0E, 0x60, 0x58, 0x5A, 0x02, 0x18, 0x38, 0x62, 0x32, 0x0C, 0x54, 0x42,
        0x3A, 0x3C, 0x48, 0x5E, 0x1A, 0x44, 0x14, 0x52, 0x36, 0x24, 0x1C, 0x1E, 0x2A, 0x40, 0x60, 0x26, 0x5A, 0x34, 0x18, 0x06, 0x62,
        0x00, 0x0C, 0x22, 0x42, 0x08, 0x3C, 0x16, 0x5E, 0x4C, 0x44, 0x46, 0x52, 0x04, 0x24, 0x4E, 0x1E, 0x5C, 0x40, 0x2E, 0x26, 0x28,
        0x34, 0x4A, 0x06, 0x30, 0x00, 0x3E, 0x22, 0x10, 0x08, 0x0A, 0x16, 0x2C, 0x4C, 0x12, 0x46, 0x20, 0x04, 0x56, 0x4E, 0x50, 0x5C,
        0x0E, 0x2E, 0x58, 0x28, 0x02, 0x4A, 0x38, 0x30, 0x32, 0x3E, 0x54, 0x10, 0x3A, 0x0A, 0x48, 0x2C, 0x1A, 0x12, 0x14, 0x20, 0x36,
        0x56, 0x1C, 0x50, 0x2A, 0x0E, 0x60, 0x58, 0x5A, 0x02, 0x18, 0x38, 0x62, 0x32, 0x0C, 0x54, 0x42, 0x3A, 0x3C, 0x48, 0x5E, 0x1A,
        0x44, 0x14, 0x52, 0x36, 0x24, 0x1C, 0x1E, 0x2A, 0x40, 0x60, 0x26, 0x5A, 0x34, 0x18, 0x06, 0x62, 0x00, 0x0C, 0x22, 0x42, 0x08,
        0x3C, 0x16, 0x5E, 0x4C, 0x44, 0x46, 0x52, 0x04, 0x24, 0x4E, 0x1E, 0x5C, 0x40, 0x2E, 0x26, 0x28, 0x34, 0x4A, 0x06, 0x30, 0x00,
        0x3E, 0x22, 0x10, 0x08, 0x0A, 0x16, 0x2C, 0x4C, 0x12, 0x46, 0x20, 0x04, 0x56, 0x4E, 0x50, 0x5C, 0x0E, 0x2E, 0x58, 0x28, 0x02,
        0x4A, 0x38, 0x30, 0x32, 0x3E, 0x54, 0x10, 0x3A, 0x0A, 0x48, 0x2C, 0x1A, 0x12, 0x14, 0x20, 0x36, 0x56, 0x1C, 0x50, 0x2A, 0x0E,
        0x60, 0x58, 0x5A, 0x02, 0x18, 0x38, 0x62, 0x32, 0x0C, 0x54, 0x42, 0x3A, 0x3C, 0x48, 0x5E, 0x1A, 0x44, 0x14, 0x52, 0x36, 0x24,
        0x1C, 0x1E, 0x2A, 0x40, 0x60, 0x26, 0x5A, 0x34, 0x18, 0x06, 0x62, 0x00, 0x0C, 0x22, 0x42, 0x08, 0x3C, 0x16, 0x5E, 0x4C, 0x44,
        0x46, 0x52, 0x04, 0x24, 0x4E, 0x1E, 0x5C, 0x40, 0x2E, 0x26, 0x28, 0x34, 0x4A, 0x06, 0x30, 0x00, 0x3E, 0x22, 0x10, 0x08, 0x0A,
        0x16, 0x2C, 0x4C, 0x12, 0x46, 0x20, 0x04, 0x56, 0x4E, 0x50, 0x5C, 0x0E, 0x2E, 0x58, 0x28, 0x02, 0x4A, 0x38, 0x30, 0x32, 0x3E,
        0x54, 0x10, 0x3A, 0x0A, 0x48, 0x2C, 0x1A, 0x12, 0x14, 0x20, 0x36, 0x56, 0x1C, 0x50, 0x2A, 0x0E, 0x60, 0x58, 0x5A, 0x02, 0x18,
        0x38, 0x62, 0x32, 0x0C, 0x54, 0x42, 0x3A, 0x3C, 0x48, 0x5E, 0x1A, 0x44, 0x14, 0x52, 0x36, 0x24, 0x1C, 0x1E, 0x2A, 0x40, 0x60,
        0x26, 0x5A, 0x34, 0x18, 0x06, 0x62, 0x00, 0x0C, 0x22, 0x42, 0x08, 0x3C, 0x16, 0x5E, 0x4C, 0x44, 0x46, 0x52, 0x04, 0x24, 0x4E,
        0x1E, 0x5C, 0x40, 0x2E, 0x26, 0x28, 0x34, 0x4A, 0x06, 0x30, 0x06, 0x00, 0x0A, 0x64, 0x6F, 0x75, 0x62, 0x6C, 0x65, 0x54, 0x65,
        0x73, 0x74, 0x3F, 0xDF, 0x8F, 0x6B, 0xBB, 0xFF, 0x6A, 0x5E, 0x00 // data from https://wiki.vg/NBT#bigtest.nbt (decompressed)
    };

    ReadIterator iter = data.begin();
    size_t length = data.size();

    NBT::Value nbt = ReadData<NBT::Value>(iter, length);

    SECTION("General")
    {
        CHECK(nbt.HasData());
        CHECK(nbt.GetName() == "Level");
        CHECK(nbt.is<NBT::TagCompound>());
        CHECK(nbt.size() == 11);
    }

    SECTION("Compound")
    {
        CHECK(nbt["nested compound test"].is<NBT::TagCompound>());
        CHECK(nbt["nested compound test"].size() == 2);
    }

    SECTION("Int")
    {
        CHECK(nbt["intTest"].is<NBT::TagInt>());
        CHECK(nbt["intTest"].get<int>() == 2147483647);
        CHECK(nbt["intTest"].get<NBT::TagInt>() == 2147483647);
    }

    SECTION("Byte")
    {
        CHECK(nbt["byteTest"].is<NBT::TagByte>());
        CHECK(nbt["byteTest"].get<NBT::TagByte>() == 127);
    }

    SECTION("String")
    {
        CHECK(nbt["stringTest"].is<NBT::TagString>());
        CHECK(nbt["stringTest"].get<NBT::TagString>().size() == 41);
        CHECK(nbt["stringTest"].get<NBT::TagString>().substr(0, 11) == "HELLO WORLD");
    }

    SECTION("List")
    {
        CHECK(nbt["listTest (long)"].is<NBT::TagList>());
        CHECK(nbt["listTest (long)"].is_list_of<NBT::TagLong>());
        CHECK(nbt["listTest (long)"].get<NBT::TagList>().size() == 5);
        CHECK(nbt["listTest (long)"].as_list_of<NBT::TagLong>()[3] == 14);
        CHECK(nbt["listTest (long)"].get<NBT::TagList>().is_of<NBT::TagLong>());

        CHECK(nbt["listTest (compound)"].is<NBT::TagList>());
        CHECK(nbt["listTest (compound)"].get<NBT::TagList>().size() == 2);
    }

    SECTION("Double")
    {
        CHECK(nbt["doubleTest"].is<NBT::TagDouble>());
        CHECK(nbt["doubleTest"].get<NBT::TagDouble>() == 0.49312871321823148);
    }

    SECTION("Float")
    {
        CHECK(nbt["floatTest"].is<NBT::TagFloat>());
        CHECK(nbt["floatTest"].get<NBT::TagFloat>() == 0.49823147058486938f);
    }

    SECTION("Long")
    {
        CHECK(nbt["longTest"].is<NBT::TagLong>());
        CHECK(nbt["longTest"].get<NBT::TagLong>() == 9223372036854775807L);
    }

    SECTION("ByteArray")
    {
        CHECK(nbt["byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"].is<NBT::TagByteArray>());
        CHECK(nbt["byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"].get<NBT::TagByteArray>().size() == 1000);
        CHECK(nbt["byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"].get<NBT::TagByteArray>()[2] == 34);
    }

    SECTION("Short")
    {
        CHECK(nbt["shortTest"].is<NBT::TagShort>());
        CHECK(nbt["shortTest"].get<NBT::TagShort>() == 32767);
    }

    SECTION("Nested")
    {
        CHECK(nbt["nested compound test"]["egg"]["name"].is<std::string>());
        CHECK(nbt["nested compound test"]["egg"]["name"].get<std::string>() == "Eggbert");

        CHECK(nbt["nested compound test"]["ham"]["value"].is<float>());
        CHECK(nbt["nested compound test"]["ham"]["value"].get<float>() == 0.75f);

        CHECK(nbt["listTest (compound)"].as_list_of<NBT::TagCompound>()[1]["created-on"].is<NBT::TagLong>());
        CHECK(nbt["listTest (compound)"].as_list_of<NBT::TagCompound>()[1]["created-on"].get<NBT::TagLong>() == 1264099775885L);
    }
}

TEST_CASE("Compressed bigtest nbt")
{
    std::vector<unsigned char> data = {
        0x1F, 0x8B, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xED, 0x54, 0xCF, 0x4F, 0x1A, 0x41, 0x14, 0x7E, 0xC2, 0x02, 0xCB,
        0x96, 0x82, 0xB1, 0xC4, 0x10, 0x63, 0xCC, 0xAB, 0xB5, 0x84, 0xA5, 0xDB, 0xCD, 0x42, 0x11, 0x89, 0xB1, 0x88, 0x16, 0x2C, 0x9A,
        0x0D, 0x1A, 0xD8, 0xA8, 0x31, 0x86, 0xB8, 0x2B, 0xC3, 0x82, 0x2E, 0xBB, 0x66, 0x77, 0xB0, 0xF1, 0xD4, 0x4B, 0x7B, 0x6C, 0x7A,
        0xEB, 0x3F, 0xD3, 0x23, 0x7F, 0x43, 0xCF, 0xBD, 0xF6, 0xBF, 0xA0, 0xC3, 0x2F, 0x7B, 0x69, 0xCF, 0xBD, 0xF0, 0x32, 0xC9, 0xF7,
        0xE6, 0xBD, 0x6F, 0xE6, 0x7B, 0x6F, 0x26, 0x79, 0x02, 0x04, 0x54, 0x72, 0x4F, 0x2C, 0x0E, 0x78, 0xCB, 0xB1, 0x4D, 0x8D, 0x78,
        0xF4, 0xE3, 0x70, 0x62, 0x3E, 0x08, 0x7B, 0x1D, 0xC7, 0xA5, 0x93, 0x18, 0x0F, 0x82, 0x47, 0xDD, 0xEE, 0x84, 0x02, 0x62, 0xB5,
        0xA2, 0xAA, 0xC7, 0x78, 0x76, 0x5C, 0x57, 0xCB, 0xA8, 0x55, 0x0F, 0x1B, 0xC8, 0xD6, 0x1E, 0x6A, 0x95, 0x86, 0x86, 0x0D, 0xAD,
        0x7E, 0x58, 0x7B, 0x8F, 0x83, 0xCF, 0x83, 0x4F, 0x83, 0x6F, 0xCF, 0x03, 0x10, 0x6E, 0x5B, 0x8E, 0x3E, 0xBE, 0xA5, 0x38, 0x4C,
        0x64, 0xFD, 0x10, 0xEA, 0xDA, 0x74, 0xA6, 0x23, 0x40, 0xDC, 0x66, 0x2E, 0x69, 0xE1, 0xB5, 0xD3, 0xBB, 0x73, 0xFA, 0x76, 0x0B,
        0x29, 0xDB, 0x0B, 0xE0, 0xEF, 0xE8, 0x3D, 0x1E, 0x38, 0x5B, 0xEF, 0x11, 0x08, 0x56, 0xF5, 0xDE, 0x5D, 0xDF, 0x0B, 0x40, 0xE0,
        0x5E, 0xB7, 0xFA, 0x64, 0xB7, 0x04, 0x00, 0x8C, 0x41, 0x4C, 0x73, 0xC6, 0x08, 0x55, 0x4C, 0xD3, 0x20, 0x2E, 0x7D, 0xA4, 0xC0,
        0xC8, 0xC2, 0x10, 0xB3, 0xBA, 0xDE, 0x58, 0x0B, 0x53, 0xA3, 0xEE, 0x44, 0x8E, 0x45, 0x03, 0x30, 0xB1, 0x27, 0x53, 0x8C, 0x4C,
        0xF1, 0xE9, 0x14, 0xA3, 0x53, 0x8C, 0x85, 0xE1, 0xD9, 0x9F, 0xE3, 0xB3, 0xF2, 0x44, 0x81, 0xA5, 0x7C, 0x33, 0xDD, 0xD8, 0xBB,
        0xC7, 0xAA, 0x75, 0x13, 0x5F, 0x28, 0x1C, 0x08, 0xD7, 0x2E, 0xD1, 0x59, 0x3F, 0xAF, 0x1D, 0x1B, 0x60, 0x21, 0x59, 0xDF, 0xFA,
        0xF1, 0x05, 0xFE, 0xC1, 0xCE, 0xFC, 0x9D, 0xBD, 0x00, 0xBC, 0xF1, 0x40, 0xC9, 0xF8, 0x85, 0x42, 0x40, 0x46, 0xFE, 0x9E, 0xEB,
        0xEA, 0x0F, 0x93, 0x3A, 0x68, 0x87, 0x60, 0xBB, 0xEB, 0x32, 0x37, 0xA3, 0x28, 0x0A, 0x8E, 0xBB, 0xF5, 0xD0, 0x69, 0x63, 0xCA,
        0x4E, 0xDB, 0xE9, 0xEC, 0xE6, 0xE6, 0x2B, 0x3B, 0xBD, 0x25, 0xBE, 0x64, 0x49, 0x09, 0x3D, 0xAA, 0xBB, 0x94, 0xFD, 0x18, 0x7E,
        0xE8, 0xD2, 0x0E, 0xDA, 0x6F, 0x15, 0x4C, 0xB1, 0x68, 0x3E, 0x2B, 0xE1, 0x9B, 0x9C, 0x84, 0x99, 0xBC, 0x84, 0x05, 0x09, 0x65,
        0x59, 0x16, 0x45, 0x00, 0xFF, 0x2F, 0x28, 0xAE, 0x2F, 0xF2, 0xC2, 0xB2, 0xA4, 0x2E, 0x1D, 0x20, 0x77, 0x5A, 0x3B, 0xB9, 0x8C,
        0xCA, 0xE7, 0x29, 0xDF, 0x51, 0x41, 0xC9, 0x16, 0xB5, 0xC5, 0x6D, 0xA1, 0x2A, 0xAD, 0x2C, 0xC5, 0x31, 0x7F, 0xBA, 0x7A, 0x92,
        0x8E, 0x5E, 0x9D, 0x5F, 0xF8, 0x12, 0x05, 0x23, 0x1B, 0xD1, 0xF6, 0xB7, 0x77, 0xAA, 0xCD, 0x95, 0x72, 0xBC, 0x9E, 0xDF, 0x58,
        0x5D, 0x4B, 0x97, 0xAE, 0x92, 0x17, 0xB9, 0x44, 0xD0, 0x80, 0xC8, 0xFA, 0x3E, 0xBF, 0xB3, 0xDC, 0x54, 0xCB, 0x07, 0x75, 0x6E,
        0xA3, 0xB6, 0x76, 0x59, 0x92, 0x93, 0xA9, 0xDC, 0x51, 0x50, 0x99, 0x6B, 0xCC, 0x35, 0xE6, 0x1A, 0xFF, 0x57, 0x23, 0x08, 0x42,
        0xCB, 0xE9, 0x1B, 0xD6, 0x78, 0xC2, 0xEC, 0xFE, 0xFC, 0x7A, 0xFB, 0x7D, 0x78, 0xD3, 0x84, 0xDF, 0xD4, 0xF2, 0xA4, 0xFB, 0x08,
        0x06, 0x00, 0x00 // data from https://wiki.vg/NBT#bigtest.nbt
    };

    ReadIterator iter = data.begin();
    size_t length = data.size();

#if !USE_COMPRESSION
    CHECK_THROWS(ReadData<NBT::Value>(iter, length));
#else
    NBT::Value nbt = ReadData<NBT::Value>(iter, length);

    SECTION("General")
    {
        CHECK(nbt.HasData());
        CHECK(nbt.GetName() == "Level");
        CHECK(nbt.is<NBT::TagCompound>());
        CHECK(nbt.size() == 11);
    }

    SECTION("Compound")
    {
        CHECK(nbt["nested compound test"].is<NBT::TagCompound>());
        CHECK(nbt["nested compound test"].size() == 2);
    }

    SECTION("Int")
    {
        CHECK(nbt["intTest"].is<NBT::TagInt>());
        CHECK(nbt["intTest"].get<int>() == 2147483647);
        CHECK(nbt["intTest"].get<NBT::TagInt>() == 2147483647);
    }

    SECTION("Byte")
    {
        CHECK(nbt["byteTest"].is<NBT::TagByte>());
        CHECK(nbt["byteTest"].get<NBT::TagByte>() == 127);
    }

    SECTION("String")
    {
        CHECK(nbt["stringTest"].is<NBT::TagString>());
        CHECK(nbt["stringTest"].get<NBT::TagString>().size() == 41);
        CHECK(nbt["stringTest"].get<NBT::TagString>().substr(0, 11) == "HELLO WORLD");
    }

    SECTION("List")
    {
        CHECK(nbt["listTest (long)"].is<NBT::TagList>());
        CHECK(nbt["listTest (long)"].is_list_of<NBT::TagLong>());
        CHECK(nbt["listTest (long)"].get<NBT::TagList>().size() == 5);
        CHECK(nbt["listTest (long)"].as_list_of<NBT::TagLong>()[3] == 14);
        CHECK(nbt["listTest (long)"].get<NBT::TagList>().is_of<NBT::TagLong>());

        CHECK(nbt["listTest (compound)"].is<NBT::TagList>());
        CHECK(nbt["listTest (compound)"].get<NBT::TagList>().size() == 2);
    }

    SECTION("Double")
    {
        CHECK(nbt["doubleTest"].is<NBT::TagDouble>());
        CHECK(nbt["doubleTest"].get<NBT::TagDouble>() == 0.49312871321823148);
    }

    SECTION("Float")
    {
        CHECK(nbt["floatTest"].is<NBT::TagFloat>());
        CHECK(nbt["floatTest"].get<NBT::TagFloat>() == 0.49823147058486938f);
    }

    SECTION("Long")
    {
        CHECK(nbt["longTest"].is<NBT::TagLong>());
        CHECK(nbt["longTest"].get<NBT::TagLong>() == 9223372036854775807L);
    }

    SECTION("ByteArray")
    {
        CHECK(nbt["byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"].is<NBT::TagByteArray>());
        CHECK(nbt["byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"].get<NBT::TagByteArray>().size() == 1000);
        CHECK(nbt["byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"].get<NBT::TagByteArray>()[2] == 34);
    }

    SECTION("Short")
    {
        CHECK(nbt["shortTest"].is<NBT::TagShort>());
        CHECK(nbt["shortTest"].get<NBT::TagShort>() == 32767);
    }

    SECTION("Nested")
    {
        CHECK(nbt["nested compound test"]["egg"]["name"].is<std::string>());
        CHECK(nbt["nested compound test"]["egg"]["name"].get<std::string>() == "Eggbert");

        CHECK(nbt["nested compound test"]["ham"]["value"].is<float>());
        CHECK(nbt["nested compound test"]["ham"]["value"].get<float>() == 0.75f);

        CHECK(nbt["listTest (compound)"].as_list_of<NBT::TagCompound>()[1]["created-on"].is<NBT::TagLong>());
        CHECK(nbt["listTest (compound)"].as_list_of<NBT::TagCompound>()[1]["created-on"].get<NBT::TagLong>() == 1264099775885L);
    }
#endif
}

TEST_CASE("Unnamed NBT")
{
    std::vector<unsigned char> data = {
        0x0A, // TagCompound
#if PROTOCOL_VERSION < 764 /* < 1.20.2 */
        0x00, 0x0B, // Name length
        0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, // Name
#endif
        0x08, // TagString
        0x00, 0x04, // Name length
        0x6E, 0x61, 0x6D, 0x65, // Name
        0x00, 0x09, // String length
        0x42, 0x61, 0x6E, 0x61, 0x6E, 0x72, 0x61, 0x6D, 0x61, // String content
        0x00 // TagEnd
    };
    ReadIterator iter = data.begin();
    size_t length = data.size();

    NBT::Value nbt = ReadData<NBT::UnnamedValue>(iter, length);

    CHECK(nbt.HasData());
    CHECK(nbt.is<NBT::TagCompound>());
    CHECK(nbt.size() == 1);
    CHECK(nbt.GetName() == "");
    CHECK(nbt["name"].is<NBT::TagString>());
    CHECK(nbt["name"].get<NBT::TagString>() == "Bananrama");

    std::vector<unsigned char> serialized;
    serialized.reserve(data.size());

    WriteData<NBT::UnnamedValue>(nbt, serialized);

    // Name has been removed
#if PROTOCOL_VERSION < 764 /* < 1.20.2 */
    CHECK(serialized.size() == data.size() - 11);
#else
    CHECK(serialized == data);
#endif
}
