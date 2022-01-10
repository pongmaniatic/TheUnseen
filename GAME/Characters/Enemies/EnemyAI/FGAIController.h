// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyState.h"
#include "FGAISpawnPoint.h"

#include "FGEnemyStateMachine.h"
#include "FGAIController.generated.h"

class UFGSensingManager;
class AFGCharacter;
enum class EEnemyState : uint8;
class UAISenseConfig_Sight;
class UAISense_Sight;
class AFGEnemyBase;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class GAME_API AFGAIController : public AAIController
{
	GENERATED_BODY()

public:
	AFGAIController();

protected:
	UPROPERTY(VisibleInstanceOnly, Category=AI)
	AFGEnemyBase* Enemy;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	UFUNCTION()
	void OnBeginPlayerEnd();

	UFUNCTION()
	void OnArrivedAtTargetDestination();

	FTimerHandle AITimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	UFGEnemyStateMachine* StateMachine;

	//EVENTS//
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGotLineOfSight();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnLostLineOfSight();

	UFUNCTION()
	void OnLostLineOfSight();
	UFUNCTION()
	void OnGotLineOfSight();

	UFUNCTION()
	void HandleOnStateChange(EEnemyState NewEnemyState);
	UFUNCTION()
	void HandleStateOnSenseUpdate(EEnemyState CurrentState);

	//SENSING//
	UPROPERTY()
	UFGSensingManager* SensingManager;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AISightRadius = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AISightMaxAge = 10.f;
	//AI LOGIC //
	UPROPERTY(VisibleInstanceOnly)
	bool bIsPlayerVisible = false;
	UPROPERTY(VisibleInstanceOnly, Transient)
	FVector PlayerLastKnownPosition;
	UPROPERTY(VisibleInstanceOnly)
	AFGCharacter* Player;
	UFUNCTION()
	AFGAISpawnPoint* GetPoint(bool bIsBestPoint);
	UPROPERTY()
	bool bShouldActivateLogic = false;
	UFUNCTION(BlueprintCallable)
	void SetAILogicActive(bool bIsActive);
};
