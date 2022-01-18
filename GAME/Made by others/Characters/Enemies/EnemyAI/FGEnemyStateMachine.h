// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnemyState.h"

#include "Components/ActorComponent.h"

#include "FGEnemyStateMachine.generated.h"

class AFGEnemyBase;
class AFGAIController;
class AAIController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFGOnEnemyStateChange, EEnemyState, NewEnemyState);

UCLASS(Blueprintable, BlueprintType)
class GAME_API UFGEnemyStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:
	UFGEnemyStateMachine();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, Category=AI)
	EEnemyState CurrentState = EEnemyState::IDLE;
	UPROPERTY(VisibleAnywhere, Category=AI)
	EEnemyState LastState;
	float HideUnderGroundValue = 500.f;

public:
	UPROPERTY()
	AFGEnemyBase* EnemyOwner;

	UPROPERTY()
	AFGAIController* EnemyController;

	UPROPERTY()
	FFGOnEnemyStateChange OnEnemyStateChange;

	UFUNCTION(BlueprintCallable)
	EEnemyState GetState();
	UFUNCTION(BlueprintCallable)
	void SetState(EEnemyState NewState);

	void OnEnterIdle();
	void OnEnterSearching();
	void OnEnterChasing();
	void OnEnterHolding();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void HandleNewState();
};
