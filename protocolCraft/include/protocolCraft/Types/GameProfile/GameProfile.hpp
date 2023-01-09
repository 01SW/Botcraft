#if PROTOCOL_VERSION > 758
#pragma once

#include "protocolCraft/NetworkType.hpp"

#include "protocolCraft/Types/GameProfile/GameProfileProperty.hpp"

#include <string>
#include <map>

namespace ProtocolCraft 
{
    class GameProfile : public NetworkType
    {
    public:

        GameProfile()
        {

        }

        virtual ~GameProfile() override
        {

        }


        void SetUUID(const UUID& uuid_)
        {
            uuid = uuid_;
        }

        void SetName(const std::string& name_)
        {
            name = name_;
        }

        void SetProperties(const std::map<std::string, GameProfileProperty>& properties_)
        {
            properties = properties_;
        }


        const UUID& GetUUID() const
        {
            return uuid;
        }

        const std::string& GetName() const
        {
            return name;
        }

        const std::map<std::string, GameProfileProperty>& GetProperties() const
        {
            return properties;
        }

    protected:
        virtual void ReadImpl(ReadIterator &iter, size_t &length) override
        {
            uuid = ReadData<UUID>(iter, length);
            name = ReadData<std::string>(iter, length);
            properties = ReadMap<std::string, GameProfileProperty>(iter, length,
                [](ReadIterator& i, size_t& l)
                {
                    const GameProfileProperty prop = ReadData<GameProfileProperty>(i, l);

                    return std::make_pair(prop.GetName(), prop);
                }
            );
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            WriteData<UUID>(uuid, container);
            WriteData<std::string>(name, container);
            WriteMap<std::string, GameProfileProperty>(properties, container,
                [](const std::pair<const std::string, GameProfileProperty>& p, WriteContainer& c)
                {
                    WriteData<GameProfileProperty>(p.second, c);
                }
            );
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["uuid"] = uuid;
            output["name"] = name;
            output["properties"] = Json::Array();
            for (const auto& p: properties)
            {
                output["properties"].push_back(p.second.Serialize());
            }


            return output;
        }

    private:
        UUID uuid;
        std::string name;
        std::map<std::string, GameProfileProperty> properties;
    };
} // ProtocolCraft
#endif
