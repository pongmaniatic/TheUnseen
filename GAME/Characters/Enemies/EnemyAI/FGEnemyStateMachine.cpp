// Fill out your copyright notice in the Description page of Project Settings.

#include "FGEnemyStateMachine.h"

#include "FGAIController.h"

#include "GAME/GAME.h"
#include "GAME/Characters/FGCharacter.h"
#include "GAME/Characters/Enemies/FGEnemyBase.h"

// Sets default values for this component's properties
UFGEnemyStateMachine::UFGEnemyStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

// Called when the game starts
void UFGEnemyStateMachine::BeginPlay()
{
	Super::BeginPlay();
	EnemyController = GetOwner()->GetInstigatorController<AFGAIController>();
	if (!EnemyController) { UE_DEBUG_BREAK(); }
	EnemyOwner = GetOwner()->GetInstigatorController()->GetPawn<AFGEnemyBase>();
	if (!EnemyOwner) { UE_DEBUG_BREAK(); }
	LastState = EEnemyState::HOLDLOGIC;
	// ...
}

EEnemyState UFGEnemyStateMachine::GetState() { return CurrentState; }

void UFGEnemyStateMachine::OnEnterIdle()
{
	LOG("ENTER IDLE");

	if (EnemyOwner->GetDistanceToPlayer() < 500.f || EnemyOwner->bHasLineOfSight) { EnemyOwner->MoveTowards(EnemyController->PlayerLastKnownPosition); }
	else { EnemyOwner->MoveUp_World(-HideUnderGroundValue); }
}

void UFGEnemyStateMachine::OnEnterSearching()
{
	LOG("ENTER SEARCHING");
	EnemyOwner->MoveTowards(EnemyController->GetPoint(true)->SpawnPosition);
}

void UFGEnemyStateMachine::OnEnterChasing()
{
	LOG("ENTER CHASING");
	EnemyOwner->MoveTowards(EnemyController->Player->GetActorLocation());
}

void UFGEnemyStateMachine::OnEnterHolding()
{
	LOG("ENTER HOLDING");
	EnemyOwner->MoveUp_World(-HideUnderGroundValue);
}

void UFGEnemyStateMachine::SetState(const EEnemyState NewState)
{
	if (CurrentState == NewState) { return; }
	if (LastState == EEnemyState::HOLDLOGIC) { EnemyOwner->MoveUp_World(HideUnderGroundValue); }
	if (LastState == CurrentState) { return; }
	LastState = CurrentState;
	CurrentState = NewState;

	HandleNewState();
	OnEnemyStateChange.Broadcast(CurrentState);
}

void UFGEnemyStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) { Super::TickComponent(DeltaTime, TickType, ThisTickFunction); }

void UFGEnemyStateMachine::HandleNewState()
{
	switch (CurrentState)
	{
	case EEnemyState::IDLE: OnEnterIdle();

		break;

	case EEnemyState::SEARCHING: OnEnterSearching();

		break;

	case EEnemyState::CHASING: OnEnterChasing();

		break;

	case EEnemyState::HOLDLOGIC: OnEnterHolding();
		break;

	default: break;
	}
}
