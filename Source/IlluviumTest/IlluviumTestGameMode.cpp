#include "IlluviumTestGameMode.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Visual/Actions.h"
#include "Visual/VisualUnitActor.h"

AIlluviumTestGameMode::AIlluviumTestGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AIlluviumTestGameMode::StartPlay()
{
	Super::StartPlay();

	SetCamera();
	
	Simulator.Initialize(Seed, RedUnitsNum, BlueUnitsNum, MapRadius);

	if (VisualUnitClass)
	{
		for (const SimUnit& Unit : Simulator.GetUnits())
		{
			if (AVisualUnitActor* VisualUnit = GetWorld()->SpawnActor<AVisualUnitActor>(VisualUnitClass, FTransform::Identity))
			{
				VisualUnit->HexSize = HexSize;
				VisualUnit->Initialize(Unit);
				VisualsById.Add(Unit.ID, VisualUnit);
			}
		}
	}
}

void AIlluviumTestGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeSinceLastStep += DeltaSeconds;
	
	while (TimeSinceLastStep >= TimeStepSeconds)
	{
		DoSimulationStep();
		TimeSinceLastStep -= TimeStepSeconds;
	}
}

void AIlluviumTestGameMode::DoSimulationStep()
{
	if (Simulator.IsSimulationOver())
	 	return;

	Simulator.Tick();
	
	Steps = Simulator.GetSteps();

	if (CurrentStep < 0 && Steps.Num() > 0)
	{
		CurrentStep = 0;
		PlayStep(CurrentStep);
	}
	
}

void AIlluviumTestGameMode::PlayStep(int32 Index)
{
	UE_LOG(LogTemp, Log, TEXT(" Play step %i from %i"), CurrentStep, Steps.Num());
	
	if (!Steps.IsValidIndex(Index))
	{
		Steps.Reset();
		CurrentStep = -1;
		return;
	}

	const FStep& Step = Steps[Index];
	RunningActions = Step.Num();
	
	for (const FAction& Action : Step)
	{
		AVisualUnitActor* Unit = VisualsById.Contains(Action.UnitID) ? VisualsById[Action.UnitID] : nullptr;
		if (!Unit)
		{
			RunningActions--;
			if( RunningActions == 0 )
			{
				CurrentStep++;
				PlayStep(CurrentStep);
			}
			continue;
		}

		const auto ActionFinished = FOnActionFinished::CreateLambda([this]()
		{
			RunningActions--;
			
			if (RunningActions == 0)
			{
				CurrentStep++;
				PlayStep(CurrentStep);
			}
		});

		switch (Action.Type)
		{
			case EActionType::Move:
				{
					Unit->PlayMoveTo(Action.TargetPos, ActionFinished);
					break;
				}
			case EActionType::Attack:
				{
					Unit->PlayAttackTowards(Action.TargetPos, ActionFinished);
					break;
				}
			case EActionType::Hit:
				{
					Unit->PlayHitFrom(Action.AttackerPos, ActionFinished);
					break;
				}
			case EActionType::Die:
				{
					Unit->PlayDeath(ActionFinished);
					break;
				}
		}
	}
}

void AIlluviumTestGameMode::SetCamera() const
{
	ACameraActor* Camera = nullptr;
	for (const TActorIterator<ACameraActor> It(GetWorld()); It;)
	{
		Camera = *It;
		break;
	}

	if (Camera)
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->SetViewTarget(Camera);
		}
	}
}
