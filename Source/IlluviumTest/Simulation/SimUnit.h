#pragma once
#include "CoreMinimal.h"
#include "HexCoord.h"

UENUM()
enum class ETeam : uint8
{
	Red,
	Blue
};

class ILLUVIUMTEST_API SimUnit
{

public:

	void Tick();
	bool IsInRange(const HexCoord& OtherPos, int32 Range) const;
	
	int32 ID;
	ETeam Team;
	int32 HP;
	int32 MaxHP;
	HexCoord GridPos;
	HexCoord TargetPos;
	HexCoord AttackerPos;
	int32 AttackCooldown = 0;
	bool bIsAlive = true;
	
};
