#pragma once

#include "CoreMinimal.h"
#include "IlluviumTest/Simulation/HexCoord.h"

enum class EActionType : uint8
{
	Move,
	Attack,
	Hit,
	Die
};

struct FAction
{
	int32 UnitID;
	EActionType Type;
	HexCoord TargetPos;           
	HexCoord AttackerPos;           
};

using FStep = TArray<FAction>;