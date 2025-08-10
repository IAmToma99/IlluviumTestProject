#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Simulation/BattleSimulator.h"
#include "Visual/VisualUnitActor.h"
#include "Visual/Actions.h"
#include "IlluviumTestGameMode.generated.h"

UCLASS(minimalapi)
class AIlluviumTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AIlluviumTestGameMode();

	virtual void Tick(float DeltaSeconds) override;

protected:
	
	virtual void StartPlay() override;

private:
	
	void PlayStep(int32 Index);
	void DoSimulationStep();
	
	void SetCamera() const;
	
	UPROPERTY(EditAnywhere)
	int32 Seed = 1337;
	
	UPROPERTY(EditAnywhere)
	int32 RedUnitsNum = 3;

	UPROPERTY(EditAnywhere)
	int32 BlueUnitsNum = 3;

	UPROPERTY(EditAnywhere)
	float TimeStepSeconds = 0.1f;
	
	UPROPERTY(EditAnywhere, meta=(AllowAbstract="false"))
	TSubclassOf<AVisualUnitActor> VisualUnitClass;

	UPROPERTY(EditAnywhere)
	float MapRadius = 10;
	
	UPROPERTY(EditAnywhere)
	float HexSize = 100.f;
	
	UPROPERTY()
	TMap<int32, AVisualUnitActor*> VisualsById;
	
	float TimeSinceLastStep;
	TArray<FStep> Steps;
	int32 CurrentStep = -1;
	int32 RunningActions;
	BattleSimulator Simulator;
	
};



