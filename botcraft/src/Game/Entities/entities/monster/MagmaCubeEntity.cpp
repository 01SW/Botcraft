#include "botcraft/Game/Entities/entities/monster/MagmaCubeEntity.hpp"

namespace Botcraft
{
    MagmaCubeEntity::MagmaCubeEntity()
    {

    }

    MagmaCubeEntity::~MagmaCubeEntity()
    {

    }


    std::string MagmaCubeEntity::GetName() const
    {
        return "magma_cube";
    }

    EntityType MagmaCubeEntity::GetType() const
    {
        return EntityType::MagmaCube;
    }

    double MagmaCubeEntity::GetWidth() const
    {
        return 2.04;
    }

    double MagmaCubeEntity::GetHeight() const
    {
        return 2.04;
    }


    std::string MagmaCubeEntity::GetClassName()
    {
        return "magma_cube";
    }

    EntityType MagmaCubeEntity::GetClassType()
    {
        return EntityType::MagmaCube;
    }

}
