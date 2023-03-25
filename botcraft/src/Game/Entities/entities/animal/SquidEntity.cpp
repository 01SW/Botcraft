#include "botcraft/Game/Entities/entities/animal/SquidEntity.hpp"

namespace Botcraft
{
    SquidEntity::SquidEntity()
    {

    }

    SquidEntity::~SquidEntity()
    {

    }


    std::string SquidEntity::GetName() const
    {
        return "squid";
    }

    EntityType SquidEntity::GetType() const
    {
        return EntityType::Squid;
    }

    double SquidEntity::GetWidth() const
    {
        return 0.8;
    }

    double SquidEntity::GetHeight() const
    {
        return 0.8;
    }


    std::string SquidEntity::GetClassName()
    {
        return "squid";
    }

    EntityType SquidEntity::GetClassType()
    {
        return EntityType::Squid;
    }

}
