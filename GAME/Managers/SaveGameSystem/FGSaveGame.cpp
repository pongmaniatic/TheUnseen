// Fill out your copyright notice in the Description page of Project Settings.

#include "FGSaveGame.h"

#include "FGSaveGameManager.h"

#include "GAME/Characters/Components/FGRadarTrackerComponent.h"
#include "GAME/Characters/Enemies/FGEnemyBase.h"
#include "GAME/Characters/Enemies/EnemyAI/FGRadarTrackingTarget.h"
#include "GAME/Environment/Interactable/InteractableBase.h"
#include "GAME/Managers/FGGameInstance.h"
#include "GAME/Managers/FGSensingManager.h"
#include "Kismet/GameplayStatics.h"

UFGSaveGame::UFGSaveGame() {}

bool UFGSaveGame::Init(FString _SaveName)
{
	UFGSensingManager* SensingManager = UFGSensingManager::Get();
	PlayerTransform = SensingManager->PlayerScannerComponent->GetOwner()->GetActorTransform();
	if (SaveName.IsEmpty()) { SaveName = _SaveName; }
	SaveTime = "Save time:";
	//TODO Add Time;

	for (UFGRadarTrackingTarget* Target : SensingManager->RadarTrackingTargets)
	{
		if (Target == nullptr) { continue; }
		AFGEnemyBase* EnemyBase = Cast<AFGEnemyBase>(Target->GetOwner());
		if (!EnemyBase)
		{
			AInteractableBase* Interactable = Cast<AInteractableBase>(Target->GetOwner());
			if (!Interactable) { continue; }
			Interactables.Add(Interactable);
			continue;
		}
		MonsterTransform = EnemyBase->GetActorTransform();
		return false;
	}
	GetCompletedPuzzles();
	return false;
}

void UFGSaveGame::Load()
{
	UFGSensingManager* SensingManager = UFGSensingManager::Get();
	SensingManager->PlayerScannerComponent->GetOwner()->SetActorTransform(PlayerTransform);
	SensingManager->PlayerScannerComponent->GetOwner()->SetActorRotation(PlayerTransform.GetRotation());

	for (UFGRadarTrackingTarget* Target : SensingManager->RadarTrackingTargets)
	{
		if (Target == nullptr) { continue; }
		AFGEnemyBase* EnemyBase = Cast<AFGEnemyBase>(Target->GetOwner());
		if (!EnemyBase)
		{
			AInteractableBase* Interactable = Cast<AInteractableBase>(Target->GetOwner());
			if (!Interactable) { continue; }
			continue;
		}
		EnemyBase->SetActorTransform(MonsterTransform);
	}
	UFGSaveGameManager::Get()->CompletedPuzzles = CompletedPuzzles;
	UFGSaveGameManager::Get()->ReloadPuzzles();
}

bool UFGSaveGame::GetCompletedPuzzles()
{
	CompletedPuzzles = UFGSaveGameManager::Get()->CompletedPuzzles;
	if (CompletedPuzzles.Num() > 0) { return true; }
	return false;
}
