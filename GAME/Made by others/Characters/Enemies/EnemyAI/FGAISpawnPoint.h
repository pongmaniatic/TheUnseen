// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "FGAISpawnPoint.generated.h"

class AFGEnemyBase;
class UFGSensingManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFGOnRecalculateReward);

UCLASS(Blueprintable, BlueprintType)

class GAME_API AFGAISpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties

	AFGAISpawnPoint();

private:
	UPROPERTY(VisibleInstanceOnly, Category=AI)

	bool bIsVisibleByPlayer = false;

	UPROPERTY(EditDefaultsOnly, Category=AI)

	float BaseRewardValue = 100;

	UPROPERTY(EditDefaultsOnly, Category=AI)

	float MaxRewardValue = 10000;

	UFUNCTION()
	void HandleEndPlay(AActor* EndActor, EEndPlayReason::Type EndPlayReason);

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category=AI)
	bool IsVisibleByPlayer();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=AI)
	float PlayerIsCloseDistance = 1000.f;

	UPROPERTY(VisibleInstanceOnly, Category=AI)
	float Reward = 10;

	UFUNCTION()
	void CalculateReward();

	UPROPERTY(Transient, VisibleInstanceOnly, Category=AI, Transient)
	int PlayerFrequencyCounter = 0;

	UPROPERTY(VisibleInstanceOnly, Category=AI, Transient)
	UFGSensingManager* SensingManager;

	UPROPERTY(BlueprintCallable, Category=AI, Transient)
	FFGOnRecalculateReward OnRecalculateReward;

	UPROPERTY()
	bool bIsGoodForSpawn = true;

	UFUNCTION(BlueprintCallable, Category=AI)
	bool IsGoodForSpawn();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=AI)
	float DistanceToPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=AI)
	FVector SpawnPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=AI)
	FVector SpawnRotationDirection;

	UPROPERTY(Transient, VisibleAnywhere, Category=AI)
	float TimeSinceLastUpdate = 5001;

	UPROPERTY(EditAnywhere, Category=AI)
	float UpdateTimeDelay = 5.f;

	UPROPERTY(VisibleInstanceOnly, Category=AI)
	AFGEnemyBase* Enemy;

	UFUNCTION()
	void FindNewPosition();

	UFUNCTION(BlueprintCallable)
	void Update();

	UFUNCTION()
	void UpdateDistanceToPlayer();

	UFUNCTION()
	void UpdatePlayerFrequency();
};
