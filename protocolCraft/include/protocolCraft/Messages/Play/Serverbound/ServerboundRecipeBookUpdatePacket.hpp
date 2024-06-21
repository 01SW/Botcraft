#if PROTOCOL_VERSION < 737 /* < 1.16.2 */
#pragma once

#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/Identifier.hpp"

namespace ProtocolCraft
{
    class ServerboundRecipeBookUpdatePacket : public BaseMessage<ServerboundRecipeBookUpdatePacket>
    {
    public:

        static constexpr std::string_view packet_name = "Recipe Book Update";

#if PROTOCOL_VERSION < 393 /* < 1.13 */
        DECLARE_FIELDS(
            (VarInt,  VarInt, bool,    bool),
            (Purpose, Recipe, GuiOpen, FilteringCraftable)
        );
#elif PROTOCOL_VERSION < 477 /* < 1.14 */
        DECLARE_FIELDS(
            (VarInt,  Identifier, bool,    bool,               bool,           bool),
            (Purpose, Recipe,     GuiOpen, FilteringCraftable, FurnaceGuiOpen, FurnaceFilteringCraftable)
        );
#else
        DECLARE_FIELDS(
            (VarInt,  Identifier, bool,    bool,               bool,           bool,                      bool,                bool,                           bool,          bool),
            (Purpose, Recipe,     GuiOpen, FilteringCraftable, FurnaceGuiOpen, FurnaceFilteringCraftable, BlastFurnaceGuiOpen, BlastFurnaceFilteringCraftable, SmokerGuiOpen, SmokerFilteringCraftable)
        );
#endif

        GETTER_SETTER(Purpose);
        GETTER_SETTER(Recipe);
        GETTER_SETTER(GuiOpen);
        GETTER_SETTER(FilteringCraftable);
#if PROTOCOL_VERSION > 358 /* > 1.12.2 */
        GETTER_SETTER(FurnaceGuiOpen);
        GETTER_SETTER(FurnaceFilteringCraftable);
#endif
#if PROTOCOL_VERSION > 450 /* > 1.13.2 */
        GETTER_SETTER(BlastFurnaceGuiOpen);
        GETTER_SETTER(BlastFurnaceFilteringCraftable);
        GETTER_SETTER(SmokerGuiOpen);
        GETTER_SETTER(SmokerFilteringCraftable);
#endif

    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            SetPurpose(ReadData<VarInt>(iter, length));
            switch (GetPurpose())
            {
            case 0:
#if PROTOCOL_VERSION > 375 /* > 1.12.2 */
                SetRecipe(ReadData<Identifier>(iter, length));
#else
                SetRecipe(ReadData<VarInt>(iter, length));
#endif
                break;
            case 1:
                SetGuiOpen(ReadData<bool>(iter, length));
                SetFilteringCraftable(ReadData<bool>(iter, length));
#if PROTOCOL_VERSION > 358 /* > 1.12.2 */
                SetFurnaceGuiOpen(ReadData<bool>(iter, length));
                SetFurnaceFilteringCraftable(ReadData<bool>(iter, length));
#endif
#if PROTOCOL_VERSION > 450 /* > 1.13.2 */
                SetBlastFurnaceGuiOpen(ReadData<bool>(iter, length));
                SetBlastFurnaceFilteringCraftable(ReadData<bool>(iter, length));
                SetSmokerGuiOpen(ReadData<bool>(iter, length));
                SetSmokerFilteringCraftable(ReadData<bool>(iter, length));
#endif
                break;
            default:
                break;
            }
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            WriteData<VarInt>(GetPurpose(), container);
            switch (GetPurpose())
            {
            case 0:
#if PROTOCOL_VERSION > 375 /* > 1.12.2 */
                WriteData<Identifier>(GetRecipe(), container);
#else
                WriteData<VarInt>(GetRecipe(), container);
#endif
                break;
            case 1:
                WriteData<bool>(GetGuiOpen(), container);
                WriteData<bool>(GetFilteringCraftable(), container);
#if PROTOCOL_VERSION > 358 /* > 1.12.2 */
                WriteData<bool>(GetFurnaceGuiOpen(), container);
                WriteData<bool>(GetFurnaceFilteringCraftable(), container);
#endif
#if PROTOCOL_VERSION > 450 /* > 1.13.2 */
                WriteData<bool>(GetBlastFurnaceGuiOpen(), container);
                WriteData<bool>(GetBlastFurnaceFilteringCraftable(), container);
                WriteData<bool>(GetSmokerGuiOpen(), container);
                WriteData<bool>(GetSmokerFilteringCraftable(), container);
#endif
                break;
            default:
                break;
            }
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output[std::string(json_names[static_cast<size_t>(FieldsEnum::Purpose)])] = GetPurpose();
            switch (GetPurpose())
            {
            case 0:
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::Recipe)])] = GetRecipe();
                break;
            case 1:
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::GuiOpen)])] = GetGuiOpen();
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::FilteringCraftable)])] = GetFilteringCraftable();
#if PROTOCOL_VERSION > 358 /* > 1.12.2 */
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::FurnaceGuiOpen)])] = GetFurnaceGuiOpen();
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::FurnaceFilteringCraftable)])] = GetFurnaceFilteringCraftable();
#endif
#if PROTOCOL_VERSION > 450 /* > 1.13.2 */
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::BlastFurnaceGuiOpen)])] = GetBlastFurnaceGuiOpen();
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::BlastFurnaceFilteringCraftable)])] = GetBlastFurnaceFilteringCraftable();
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::SmokerGuiOpen)])] = GetSmokerGuiOpen();
                output[std::string(json_names[static_cast<size_t>(FieldsEnum::SmokerFilteringCraftable)])] = GetSmokerFilteringCraftable();
#endif
                break;
            default:
                break;
            }

            return output;
        }
    };
} //ProtocolCraft
#endif
