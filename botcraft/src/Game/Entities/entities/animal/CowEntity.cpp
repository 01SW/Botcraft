#include "botcraft/Game/Entities/entities/animal/CowEntity.hpp"

namespace Botcraft
{
    CowEntity::CowEntity()
    {

    }

    CowEntity::~CowEntity()
    {

    }


    std::string CowEntity::GetName() const
    {
        return "cow";
    }

    EntityType CowEntity::GetType() const
    {
        return EntityType::Cow;
    }

    double CowEntity::GetWidth() const
    {
        return 0.9;
    }

    double CowEntity::GetHeight() const
    {
        return 1.4;
    }


    std::string CowEntity::GetClassName()
    {
        return "cow";
    }

    EntityType CowEntity::GetClassType()
    {
        return EntityType::Cow;
    }

}
