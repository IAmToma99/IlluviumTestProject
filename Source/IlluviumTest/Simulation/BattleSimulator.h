#pragma once

#include "CoreMinimal.h"
#include "IlluviumTest/Visual/Actions.h"

class SimUnit;
/**
 * 
 */
class ILLUVIUMTEST_API BattleSimulator
{
	
public:
	
	void Initialize(int32 InSeed, int32 InRedUnitsNum, int32 InBlueUnitsNum, int32 MapRadius);
	void Tick();
	
	bool IsSimulationOver() const;

	const TArray<SimUnit>& GetUnits() const;
	const TArray<FStep>& GetSteps() const;
	
private:

	TArray<HexCoord> BuildTilesMap(int32 Radius);
	
	SimUnit* FindClosestEnemy(const SimUnit& Self);
	HexCoord ChooseBestPosToward(const HexCoord& FromPos, const HexCoord& TargetPos);

	int32 GridWidth = 100;
	int32 GridHeight = 100;
	
	int32 CurrentStep = 0;
	
	int32 AttackRange = 1;
	int32 AttackRate = 1;
	TArray<SimUnit> Units;
	int32 UnitCounter = 0;

	TArray<HexCoord> ValidTiles;
	TSet<HexCoord> ValidTilesSet;
	TSet<HexCoord> OccupiedTiles;

	TArray<FStep> Steps;

	const HexCoord HexDirections[6] = {
		HexCoord(+1,  0), HexCoord(+1, -1), HexCoord( 0, -1),
		HexCoord(-1,  0), HexCoord(-1, +1), HexCoord( 0, +1)
	};
};
