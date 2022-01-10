// Fill out your copyright notice in the Description page of Project Settings.

#include "FGAISpawnPoint.h"

#include "FGRadarTrackingTarget.h"

#include "GAME/Characters/Enemies/FGEnemyBase.h"

#include "GAME/GAME.h"
#include "GAME/Managers/FGSensingManager.h"

// Sets default values

AFGAISpawnPoint::AFGAISpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SetActorEnableCollision(false);

	SetCanBeDamaged(false);
}

// Called when the game starts or when spawned

void AFGAISpawnPoint::HandleEndPlay(AActor* EndActor, EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::LevelTransition) { return; }
	SensingManager->EnemySpawnPoints.Remove(this);
}

void AFGAISpawnPoint::BeginPlay()
{
	SensingManager = UFGSensingManager::Get();

	if (SensingManager->EnemySpawnPoints.Contains(this)) { if (! IsValid(this)) { SensingManager->EnemySpawnPoints.Remove(this); } }

	SensingManager->EnemySpawnPoints.Add(this);

	Super::BeginPlay();

	OnRecalculateReward.AddDynamic(this, &AFGAISpawnPoint::CalculateReward);

	SpawnPosition = GetActorLocation() + (GetActorForwardVector() * 10.f) + (FVector::UpVector * 5.f);

	SpawnRotationDirection = GetActorForwardVector();
	OnEndPlay.AddDynamic(this, &AFGAISpawnPoint::HandleEndPlay);
}

bool AFGAISpawnPoint::IsVisibleByPlayer()
{
	if (SensingManager->Player->GetDistanceTo(this) < PlayerIsCloseDistance) { bIsVisibleByPlayer = false; }
	else
	{
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector::UpVector * 10.f, SensingManager->Player->GetActorLocation(), ECC_Visibility)) { if (HitResult.bBlockingHit) { bIsVisibleByPlayer = false; } }
	}
	return bIsVisibleByPlayer;
}

void AFGAISpawnPoint::CalculateReward()
{
	Update();

	float AvgDistToPointOfInterest = 0;
	if (SensingManager->RadarTrackingTargets.Num() > 0)
	{
		for (auto* Target : SensingManager->RadarTrackingTargets)
		{
			if (!Target) { continue; }

			AvgDistToPointOfInterest += GetDistanceTo(Target->GetOwner()) / SensingManager->RadarTrackingTargets.Num();
		}
	}

	float RewardByPlayerDist = (MaxRewardValue - DistanceToPlayer) / BaseRewardValue;

	float RewardByPointOfInterestAvg = (MaxRewardValue - AvgDistToPointOfInterest) / BaseRewardValue;

	float RewardByPlayerFrequency = PlayerFrequencyCounter * BaseRewardValue;

	Reward = RewardByPlayerDist + RewardByPlayerFrequency + RewardByPointOfInterestAvg;

	Reward /= 3.f; //The number of parameters as a test;

	TimeSinceLastUpdate = 0.0f;
}

bool AFGAISpawnPoint::IsGoodForSpawn()
{
	if (!bIsGoodForSpawn) { return false; }

	if (bIsVisibleByPlayer) { bIsGoodForSpawn = false; }

	else { bIsGoodForSpawn = true; }

	return bIsGoodForSpawn;
}

void AFGAISpawnPoint::FindNewPosition()
{
	FVector Position = GetActorLocation();

	FVector DeltaMove = Position + GetActorForwardVector() * 2.f;

	AddActorWorldOffset(DeltaMove, true);

	bIsGoodForSpawn = true;

	IsGoodForSpawn();
}

void AFGAISpawnPoint::Update()
{
	if (!bIsGoodForSpawn)
	{
		FindNewPosition();

		return;
	}

	UpdateDistanceToPlayer();

	UpdatePlayerFrequency();

	SpawnPosition = GetActorLocation();

	SpawnRotationDirection = GetActorForwardVector();
}

void AFGAISpawnPoint::UpdateDistanceToPlayer() { DistanceToPlayer = GetDistanceTo(SensingManager->Player); }

void AFGAISpawnPoint::UpdatePlayerFrequency() { if (DistanceToPlayer < PlayerIsCloseDistance) { PlayerFrequencyCounter ++; } }
