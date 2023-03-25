#include "botcraft/Game/Entities/entities/vehicle/MinecartHopperEntity.hpp"

namespace Botcraft
{
    MinecartHopperEntity::MinecartHopperEntity()
    {

    }

    MinecartHopperEntity::~MinecartHopperEntity()
    {

    }


    std::string MinecartHopperEntity::GetName() const
    {
        return "hopper_minecart";
    }

    EntityType MinecartHopperEntity::GetType() const
    {
        return EntityType::MinecartHopper;
    }

    double MinecartHopperEntity::GetWidth() const
    {
        return 0.98;
    }

    double MinecartHopperEntity::GetHeight() const
    {
        return 0.7;
    }


    std::string MinecartHopperEntity::GetClassName()
    {
        return "hopper_minecart";
    }

    EntityType MinecartHopperEntity::GetClassType()
    {
        return EntityType::MinecartHopper;
    }

}
