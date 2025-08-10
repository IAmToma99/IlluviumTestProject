#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ILLUVIUMTEST_API HexCoord
{
	
public:
	
	int32 Q;
	int32 R;

	HexCoord() : Q(0), R(0) {}
	HexCoord(int32 InQ, int32 InR) : Q(InQ), R(InR) {}
	
	FORCEINLINE bool operator==(const HexCoord& Other) const
	{
		return Q == Other.Q && R == Other.R;
	}
	
	int32 DistanceTo(const HexCoord& Other) const
	{
		const int32 QDistance = FMath::Abs(Q - Other.Q);
		const int32 RDistance = FMath::Abs(R - Other.R);
		const int32 SDistance = FMath::Abs((-Q - R) - (-Other.Q - Other.R));
		return FMath::Max3(QDistance, RDistance, SDistance);
	}

	HexCoord StepTo(const HexCoord& Target) const
	{
		const int32 QDistance = Target.Q - Q;
		const int32 RDistance = Target.R - R;
		return HexCoord(Q + FMath::Clamp(QDistance, -1, 1), R + FMath::Clamp(RDistance, -1, 1));
	}
};

FORCEINLINE uint32 GetTypeHash(const HexCoord& C)
{
	return HashCombine(::GetTypeHash(C.Q), ::GetTypeHash(C.R));
}
