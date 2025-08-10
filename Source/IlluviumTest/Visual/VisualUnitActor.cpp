#include "VisualUnitActor.h"

AVisualUnitActor::AVisualUnitActor()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
}

void AVisualUnitActor::Initialize(const SimUnit& SimData)
{
    CurrentWorldTarget = HexToWorld(SimData.GridPos);
	SetActorLocation(CurrentWorldTarget);

	UMaterialInstanceDynamic* DynMat = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMat)
	{
		if (SimData.Team == ETeam::Red)
			DynMat->SetVectorParameterValue("Color", FLinearColor::Red);
		else
			DynMat->SetVectorParameterValue("Color", FLinearColor::Blue);
	}
}

void AVisualUnitActor::PlayMoveTo(const HexCoord& Target, FOnActionFinished ActionFinished)
{
	OnActionFinished = ActionFinished;
	StartPos = GetActorLocation();
	EndPos = HexToWorld(Target);
	AnimState = EAnimState::Move;
	bAnimating = true;

	UE_LOG(LogTemp, Log, TEXT("Play MoveTo %s"), *GetName())
}

void AVisualUnitActor::PlayAttackTowards(const HexCoord& EnemyHexPos, FOnActionFinished ActionFinished)
{
	OnActionFinished = ActionFinished;
	StartPos = GetActorLocation();
	const FVector EnemyPos = HexToWorld(EnemyHexPos);
	const FVector Direction = (EnemyPos - StartPos).GetSafeNormal();
	MidPos = StartPos + Direction * Nudge;
	EndPos = StartPos;
	AnimState = EAnimState::AttackForward;
	bAnimating = true;

	UE_LOG(LogTemp, Log, TEXT("Play AttackTowards %s"), *GetName())

}

void AVisualUnitActor::PlayHitFrom(const HexCoord& EnemyHexPos, FOnActionFinished ActionFinished)
{
	OnActionFinished = ActionFinished;
	StartPos = GetActorLocation();
	const FVector EnemyPos = HexToWorld(EnemyHexPos);
	const FVector Direction = (StartPos - EnemyPos).GetSafeNormal();
	MidPos = StartPos + Direction * Nudge;
	EndPos = StartPos;
	AnimState = EAnimState::HitBack;
	bAnimating = true;

	UE_LOG(LogTemp, Log, TEXT("Play HitFrom %s"), *GetName())
}

void AVisualUnitActor::PlayDeath(FOnActionFinished ActionFinished)
{
	OnActionFinished = ActionFinished;
	AnimState = EAnimState::Death;
	bAnimating = true;

	UE_LOG(LogTemp, Log, TEXT("PlayDeath %s"), *GetName())
}

void AVisualUnitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!bAnimating)
		return;

    switch (AnimState)
    {
	    case EAnimState::Move:
    		{
    			const FVector NewPos = FMath::VInterpConstantTo(GetActorLocation(), EndPos, DeltaTime, Speed);
    			SetActorLocation(NewPos);
    			
    			if (FVector::DistSquared(NewPos, EndPos) < 0.2f)
    			{
    				bAnimating = false;
    				AnimState = EAnimState::None;
    				
    				if (OnActionFinished.IsBound())
    					OnActionFinished.Execute();
    			}
    			break;
    		}
		case EAnimState::AttackForward:
    		{
    			const FVector NewPos = FMath::VInterpTo(GetActorLocation(), MidPos, DeltaTime, Speed/10);
    			SetActorLocation(NewPos);
    			
    			if (FVector::DistSquared(NewPos, MidPos) < 0.2f)
    				AnimState = EAnimState::AttackBack;
    			
    			break;
    		}
	    case EAnimState::AttackBack:
    		{
    			const FVector NewPos = FMath::VInterpTo(GetActorLocation(), EndPos, DeltaTime, Speed/10);
    			SetActorLocation(NewPos);
	    			
    			if (FVector::DistSquared(NewPos, EndPos) < 0.2f)
    			{
    				bAnimating = false;
    				AnimState = EAnimState::None;
    				
    				if (OnActionFinished.IsBound())
    				{
    					OnActionFinished.Execute();
    				}
    			}
    			break;
    		}
	    case EAnimState::HitBack:
    		{
    			const FVector NewPos = FMath::VInterpTo(GetActorLocation(), MidPos, DeltaTime, Speed/10);
    			SetActorLocation(NewPos);
    			
    			if (FVector::DistSquared(NewPos, MidPos) < 0.2f)
    				AnimState = EAnimState::HitReturn;
	    		
    			break;
    		}
	    case EAnimState::HitReturn:
    		{
    			const FVector NewPos = FMath::VInterpTo(GetActorLocation(), EndPos, DeltaTime, Speed/10);
    			SetActorLocation(NewPos);
	    			
    			if (FVector::DistSquared(NewPos, EndPos) < 0.2f)
    			{
    				bAnimating = false;
    				AnimState = EAnimState::None;
    				
    				if (OnActionFinished.IsBound())
    				{
    					OnActionFinished.Execute();
    				}
    			}
    			break;
    		}
	    case EAnimState::Death:
    		{
    			const FVector NewScale = FMath::VInterpTo(GetActorScale3D(), FVector::ZeroVector, DeltaTime, Speed/10);
    			SetActorScale3D(NewScale);
	    			
    			if (NewScale.GetMin() < 0.03f)
    			{
    				Destroy();
    				bAnimating=false;
    				AnimState=EAnimState::None;
    				
    				if (OnActionFinished.IsBound())
    				{
    					OnActionFinished.Execute();
    				}
    			}
    			break;
    		}
		default: break;
    }
	
}

FVector AVisualUnitActor::HexToWorld(const HexCoord& Coord) const
{
	float X = HexSize * 1.5f * Coord.Q;
	float Y = HexSize * FMath::Sqrt(3.f) * (Coord.R + 0.5f * Coord.Q);
	return FVector(X, Y, 0.f);
}
