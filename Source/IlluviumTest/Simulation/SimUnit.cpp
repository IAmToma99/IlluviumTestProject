#include "SimUnit.h"

void SimUnit::Tick()
{
	if (!bIsAlive)
		return;

	if (AttackCooldown > 0)
		AttackCooldown--;
}

bool SimUnit::IsInRange(const HexCoord& OtherPos, int32 Range) const
{
	return GridPos.DistanceTo(OtherPos) <= Range;
}