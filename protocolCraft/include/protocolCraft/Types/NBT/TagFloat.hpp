#pragma once

#include <memory>

#include "protocolCraft/Types/NBT/Tag.hpp"

namespace ProtocolCraft
{
    class TagFloat : public Tag
    {
    public:
        TagFloat();
        virtual ~TagFloat();

        float GetValue() const;
        void SetValue(const float v);

        virtual TagType GetType() const override;

        virtual void ReadImpl(ReadIterator &iterator, size_t &length) override;
        virtual void WriteImpl(WriteContainer &container) const override;
        virtual Json::Value SerializeImpl() const override;

    private:
        float value;
    };
}
