#pragma once

#include "Engine/EngineTypes.h"

namespace BattleBlasterCollision
{
	inline constexpr ECollisionChannel AimTrace = ECC_GameTraceChannel1;
	inline constexpr ECollisionChannel LineOfSight = ECC_GameTraceChannel2;
	inline constexpr ECollisionChannel Projectile = ECC_GameTraceChannel3;
}