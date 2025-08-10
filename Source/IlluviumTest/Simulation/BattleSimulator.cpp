#include "BattleSimulator.h"
#include "SimUnit.h"

void BattleSimulator::Initialize(int32 InSeed, int32 InRedUnitsNum, int32 InBlueUnitsNum, int32 MapRadius)
{
	CurrentStep = 0;
	
	FRandomStream RandomNumberGenerator(InSeed);
	
	Units.Empty();
	UnitCounter = 0;

	BuildTilesMap(MapRadius);
	
	for (int i = 0; i < InRedUnitsNum; i++)
	{
		int32 Index = RandomNumberGenerator.RandRange(0, ValidTiles.Num() - 1);
		HexCoord UnitPos = ValidTiles[Index];
		ValidTiles.RemoveAt(Index);
		
		SimUnit Red;
		Red.ID = UnitCounter++;
		Red.Team = ETeam::Red;
		Red.HP = RandomNumberGenerator.RandRange(2, 5);
		Red.MaxHP = Red.HP;
		Red.GridPos = UnitPos;
		Units.Add(Red);
	}

	for (int i = 0; i < InBlueUnitsNum; ++i)
	{
		int32 Index = RandomNumberGenerator.RandRange(0, ValidTiles.Num() - 1);
		HexCoord Pos = ValidTiles[Index];
		ValidTiles.RemoveAt(Index);
		
		SimUnit Blue;
		Blue.ID = UnitCounter++;
		Blue.Team = ETeam::Blue;
		Blue.HP = RandomNumberGenerator.RandRange(2, 5);
		Blue.MaxHP = Blue.HP;
		Blue.GridPos = Pos;
		Units.Add(Blue);
	}
}

void BattleSimulator::Tick()
{
    for (const SimUnit& Unit : Units)
    {
        if (Unit.bIsAlive)
        {
            OccupiedTiles.Add(Unit.GridPos);
        }
    }
	
    for (SimUnit& Unit : Units)
    {
        if (!Unit.bIsAlive) 
            continue;

        if (SimUnit* ClosestEnemy = FindClosestEnemy(Unit))
        {
            Unit.TargetPos = ClosestEnemy->GridPos;

            if (Unit.IsInRange(ClosestEnemy->GridPos, AttackRange))
            {
                if (Unit.AttackCooldown == 0)
                {
                    ClosestEnemy->AttackerPos = Unit.GridPos;
                    ClosestEnemy->HP--;

                	Steps.Add({
					   { Unit.ID, EActionType::Attack, Unit.TargetPos },
					   { ClosestEnemy->ID, EActionType::Hit, HexCoord(), ClosestEnemy->AttackerPos }
				    });
                	
                    UE_LOG(LogTemp, Log, TEXT("%i was attacked by %i"), ClosestEnemy->ID, Unit.ID);

                    if (ClosestEnemy->HP <= 0)
                    {
                        ClosestEnemy->bIsAlive = false;
                    	
                    	Steps.Add({ { ClosestEnemy->ID, EActionType::Die } });
                    	
                        UE_LOG(LogTemp, Log, TEXT("%i is dead"), ClosestEnemy->ID);
                    }

                    Unit.AttackCooldown = AttackRate;
                }
            }
            else
            {
                HexCoord NextPos = ChooseBestPosToward(Unit.GridPos, ClosestEnemy->GridPos);

                if (NextPos != Unit.GridPos)
                {
                    OccupiedTiles.Remove(Unit.GridPos);
                    OccupiedTiles.Add(NextPos);
                    Unit.GridPos = NextPos;

                	Steps.Add({{ Unit.ID, EActionType::Move, Unit.GridPos }});

                    UE_LOG(LogTemp, Log, TEXT("%i Moved"), Unit.ID);
                }
            }
        }

        Unit.Tick();
    }
	
	CurrentStep++;
}

SimUnit* BattleSimulator::FindClosestEnemy(const SimUnit& Self)
{
	int32 BestDist = MAX_int32;
	SimUnit* ClosestEnemy = nullptr;

	for (SimUnit& Other : Units) 
	{
		if (!Other.bIsAlive || Other.Team == Self.Team) 
			continue;

		int32 Dist = Self.GridPos.DistanceTo(Other.GridPos);
		if (Dist < BestDist)
		{
			BestDist = Dist;
			ClosestEnemy = &Other;
		}
	}

	return ClosestEnemy;
}

HexCoord BattleSimulator::ChooseBestPosToward(const HexCoord& FromPos, const HexCoord& TargetPos)
{
	HexCoord BestStep = FromPos.StepTo(TargetPos);
	if (BestStep != FromPos && !OccupiedTiles.Contains(BestStep) && ValidTilesSet.Contains(BestStep))
	{
		return BestStep;
	}

	int32 BestDist = MAX_int32;
	HexCoord BestPos = FromPos;

	for (int i = 0; i < 6; i++)
	{
		HexCoord NewPos(FromPos.Q + HexDirections[i].Q, FromPos.R + HexDirections[i].R);
		
		if (!ValidTilesSet.Contains(NewPos))
			continue;      
		if (OccupiedTiles.Contains(NewPos))
			continue;    

		int32 Distance = NewPos.DistanceTo(TargetPos);
		if (Distance < BestDist)
		{
			BestDist = Distance;
			BestPos = NewPos;
		}
	}

	return BestPos;
};

bool BattleSimulator::IsSimulationOver() const
{
	int32 RedsAlive = 0;
	int32 BluesAlive = 0;
	
	for (const SimUnit& Unit : GetUnits())
	{
		if (!Unit.bIsAlive)
			continue;
		
		Unit.Team == ETeam::Red ? RedsAlive++ : BluesAlive++;
	}
	
	return RedsAlive == 0 || BluesAlive == 0;
}

TArray<HexCoord> BattleSimulator::BuildTilesMap(int32 Radius)
{
	ValidTiles.Empty();
	
	for (int32 Q = -Radius; Q <= Radius; Q++)
	{
		const int32 R1 = FMath::Max(-Radius, -Q - Radius);
		const int32 R2 = FMath::Min(Radius,  -Q + Radius);
		
		for (int32 R = R1; R <= R2; R++)
		{
			HexCoord H(Q,R);
			ValidTiles.Add(H);
			ValidTilesSet.Add(H);
		}
	}

	return ValidTiles;
}

const TArray<SimUnit>& BattleSimulator::GetUnits() const
{
	return Units;
}

const TArray<FStep>& BattleSimulator::GetSteps() const
{
	return Steps;
}


