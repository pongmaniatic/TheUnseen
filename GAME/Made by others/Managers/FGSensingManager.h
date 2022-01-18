// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GAME/Characters/FGCharacter.h"

#include "FGSensingManager.generated.h"

class AFGAISpawnPoint;

class AFGSoundManager;

class UFGRadarTrackerComponent;

class AFGEnemyBase;

class UFGRadarTrackingTarget;

class AFGCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFGOnScannerRequest);

UCLASS(BlueprintType, Blueprintable)

class GAME_API UFGSensingManager : public UObject
{
	GENERATED_BODY()

public:
	UFGSensingManager();

	UFUNCTION(BlueprintCallable)

	static UFGSensingManager* Get();

	//BP_Events for player state changes!

	UPROPERTY(BlueprintAssignable, Category=AI, Transient)

	FFGOnCustomEventsDelegate OnMonsterHasLineOfSight;

	UPROPERTY(BlueprintAssignable, Category=AI, Transient)

	FFGOnCustomEventsDelegate OnMonsterLostLineOfSight;

	UPROPERTY(BlueprintAssignable, Category=Scanner, Transient)

	FFGOnScannerRequest OnScannerRequest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	TArray<AFGAISpawnPoint*> EnemySpawnPoints;

	UFUNCTION(BlueprintCallable, Category=Scanner)

	void ScannerRequest();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)

	UFGRadarTrackerComponent* PlayerScannerComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)

	AFGEnemyBase* Enemy;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)

	AFGCharacter* Player;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)

	TArray<UFGRadarTrackingTarget*> RadarTrackingTargets;
};
