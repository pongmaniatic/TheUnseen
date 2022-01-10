// Fill out your copyright notice in the Description page of Project Settings.

#include "FGAIController.h"

#include "BrainComponent.h"
#include "FGAISpawnPoint.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GAME/GAME.h"
#include "GAME/Characters/Enemies/FGEnemyBase.h"
#include "GAME/Managers/FGSensingManager.h"

#include "Kismet/GameplayStatics.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "VisualLogger/VisualLogger.h"
#include "TimerManager.h"

AFGAIController::AFGAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Setup Behaviour//
	StateMachine = CreateDefaultSubobject<UFGEnemyStateMachine>(TEXT("StateMachineComponent"));
	StateMachine->SetAutoActivate(true);

	//Setup Perception//
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AISightRadius;
	SightConfig->SetMaxAge(AISightMaxAge);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AISightRadius / 10.f;
	PerceptionComponent->SetDominantSense(TSubclassOf<UAISense_Sight>());
	PerceptionComponent->ConfigureSense(*SightConfig);
}

void AFGAIController::BeginPlay()
{
	Super::BeginPlay();

	//SETTING UP EVENTS//
	SensingManager = UFGSensingManager::Get();
	SensingManager->OnMonsterHasLineOfSight.AddDynamic(this, &AFGAIController::OnGotLineOfSight);
	SensingManager->OnMonsterLostLineOfSight.AddDynamic(this, &AFGAIController::OnLostLineOfSight);
	if (StateMachine == nullptr) { UE_DEBUG_BREAK(); }
	else { StateMachine->OnEnemyStateChange.AddDynamic(this, &AFGAIController::HandleOnStateChange); }

	GetWorld()->GetTimerManager().SetTimer(AITimerHandle, this, &AFGAIController::OnBeginPlayerEnd, 1.f, false);
}

void AFGAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//UE_LOG(LogTemp, Log, TEXT("ShouldActivateLogic = %s"), bShouldActivateLogic? TEXT("true"): TEXT("false"))
}

void AFGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<AFGEnemyBase>(InPawn);
}

void AFGAIController::OnBeginPlayerEnd()
{
	bShouldActivateLogic = true;
	ensure(StateMachine!=nullptr);
	StateMachine->SetState(EEnemyState::SEARCHING);
	LOG("::: ON TIMER END:::")
}

void AFGAIController::OnArrivedAtTargetDestination()
{
	EEnemyState CurrentState = StateMachine->GetState();
	if (CurrentState == EEnemyState::SEARCHING)
	{
		if (bIsPlayerVisible) { StateMachine->SetState(EEnemyState::CHASING); }
		else { Enemy->MoveTowards(GetPoint(true)->SpawnPosition); }
	}
	if (CurrentState == EEnemyState::CHASING) { if (bIsPlayerVisible) { Enemy->MoveTowards(Player->GetActorLocation()); } }
}

void AFGAIController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	LOG("ACTORS PERCEPTION UPDATED")
	for (auto* Actor : UpdatedActors)
	{
		AFGCharacter* TempPlayer = Cast<AFGCharacter>(Actor);
		if (!TempPlayer)
		{
			if (StateMachine->GetState() == EEnemyState::HOLDLOGIC) { Enemy->SetActorLocation(GetPoint(false)->SpawnPosition); }
			continue;
		}
		Player = TempPlayer;
		PlayerLastKnownPosition = TempPlayer->GetActorLocation();
		if (FVector::Dist(Player->GetActorLocation(), Enemy->GetActorLocation()) < AISightRadius)
		{
			if (!bIsPlayerVisible)
			{
				bIsPlayerVisible = true;
				SensingManager->OnMonsterHasLineOfSight.Broadcast();
			}
		}
		else
		{
			if (bIsPlayerVisible)
			{
				bIsPlayerVisible = false;
				SensingManager->OnMonsterLostLineOfSight.Broadcast();
			}
		}

		HandleStateOnSenseUpdate(StateMachine->GetState());
	}
}

void AFGAIController::OnLostLineOfSight()
{
	if (bShouldActivateLogic)
	{
		LOG("CONTROLLER LOST LINE OF SIGHT FROM EVENT")
		if (BrainComponent && Blackboard) { GetBrainComponent()->GetBlackboardComponent()->SetValueAsBool(FName("bHasLineOfSight"), false); }
		bIsPlayerVisible = false;
		BP_OnLostLineOfSight();
	}
}

void AFGAIController::OnGotLineOfSight()
{
	if (bShouldActivateLogic)
	{
		LOG("CONTROLLER GOT LINE OF SIGHT FROM EVENT")
		//IF using behaviourtree//
		if (BrainComponent && Blackboard) { GetBrainComponent()->GetBlackboardComponent()->SetValueAsBool(FName("bHasLineOfSight"), true); }
		bIsPlayerVisible = true;
		BP_OnGotLineOfSight();
	}
}

void AFGAIController::HandleOnStateChange(EEnemyState NewEnemyState)
{
	if (bShouldActivateLogic)
	{
		LOG("CONTROLLER Handle on State Change")
		if (BrainComponent && Blackboard) { GetBrainComponent()->GetBlackboardComponent()->SetValueAsEnum(FName("EEnemyState"), static_cast<uint8>(NewEnemyState)); }
		if (NewEnemyState == EEnemyState::HOLDLOGIC)
		{
			FVector TargetLocation = GetPoint(false)->GetActorLocation();
			if (!LineOfSightTo(Player)) { Enemy->SetActorLocation(TargetLocation); }
			else { Enemy->MoveTowards(TargetLocation); }
			bShouldActivateLogic = false;
		}
	}
}

void AFGAIController::HandleStateOnSenseUpdate(EEnemyState CurrentState)
{
	if (!bShouldActivateLogic) { return; }
	if (!StateMachine) { return; }
	//TODO Remove log UE_LOG(LogTemp, Log, TEXT("Handling State on SenseUpdate: %i"), static_cast<uint8>(CurrentState))
	switch (CurrentState)
	{
	case EEnemyState::IDLE: if (bIsPlayerVisible) { StateMachine->SetState(EEnemyState::SEARCHING); }
		break;
	case EEnemyState::CHASING: if (!bIsPlayerVisible) { StateMachine->SetState(EEnemyState::SEARCHING); }
		break;
	case EEnemyState::SEARCHING: if (bIsPlayerVisible) { StateMachine->SetState(EEnemyState::CHASING); }
		break;
	case EEnemyState::HOLDLOGIC: break;
	default: LOG("NO STATE WHEN SENSE UPDATED");
		break;
	}
}

AFGAISpawnPoint* AFGAIController::GetPoint(bool bIsBestPoint)
{
	AFGAISpawnPoint* BestPoint = nullptr;
	AFGAISpawnPoint* WorstPoint = nullptr;
	LOG("GETTING POINT")
	if (!SensingManager) { SensingManager = UFGSensingManager::Get(); }
	if (SensingManager->EnemySpawnPoints.Num() == 0) { SensingManager->EnemySpawnPoints.Add(GetWorld()->SpawnActor<AFGAISpawnPoint>(GetPawn()->GetActorLocation(), GetPawn()->GetActorRotation())); }
	for (auto* Point : SensingManager->EnemySpawnPoints)
	{
		Point->CalculateReward();
		if (!BestPoint || BestPoint->Reward < Point->Reward) { BestPoint = Point; }
		else { if (!WorstPoint || WorstPoint->Reward > Point->Reward) { WorstPoint = Point; } }
	}
	if (bIsBestPoint) { return BestPoint; }
	return WorstPoint;
}

void AFGAIController::SetAILogicActive(bool bIsActive) { bShouldActivateLogic = bIsActive; }
