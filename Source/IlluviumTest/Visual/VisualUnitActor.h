#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IlluviumTest/Simulation/SimUnit.h"
#include "VisualUnitActor.generated.h"

enum class EAnimState : uint8
{
	None,
	Move,
	AttackForward,
	AttackBack,
	HitBack,
	HitReturn,
	Death
};

DECLARE_DELEGATE(FOnActionFinished)

UCLASS(Blueprintable)
class ILLUVIUMTEST_API AVisualUnitActor : public AActor
{
	GENERATED_BODY()
	
public:
	
	AVisualUnitActor();
	
	virtual void Tick(float DeltaTime) override;

	void Initialize(const SimUnit& SimData);

	void PlayMoveTo(const HexCoord& Target, FOnActionFinished ActionFinished);
	void PlayAttackTowards(const HexCoord& EnemyHexPos, FOnActionFinished ActionFinished);
	void PlayHitFrom(const HexCoord& EnemyHexPos, FOnActionFinished ActionFinished);
	void PlayDeath(FOnActionFinished ActionFinished);

	float HexSize;
	
private:

	FVector HexToWorld(const HexCoord& Coord) const;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	float Speed = 0.5f;

	UPROPERTY(EditAnywhere)
	float Nudge = 50.f;
	
	FVector CurrentWorldTarget;


	EAnimState AnimState = EAnimState::None;

	bool bAnimating;
	
	FVector StartPos;
	FVector MidPos;
	FVector EndPos;
	
	FOnActionFinished OnActionFinished;
};
