// Fill out your copyright notice in the Description page of Project Settings.

#include "FGSaveGameManager.h"

#include "GameFramework/SaveGame.h"
#include "FGSaveGame.h"

#include "GAME/GAME.h"
#include "GAME/Managers/FGPuzzleManager.h"

#include "Kismet/GameplayStatics.h"

UFGSaveGameManager::UFGSaveGameManager() {}

void UFGSaveGameManager::SaveGame(FString SaveName)
{
	UFGSaveGame* SaveGame = Cast<UFGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));

	SaveGame->Init(SaveName);

	UGameplayStatics::SaveGameToSlot(Cast<USaveGame>(SaveGame), SaveGame->SaveName, 0);
}

void UFGSaveGameManager::DeleteSaveGame(FString SaveName)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0)) { UGameplayStatics::DeleteGameInSlot(SaveName, 0); }
}

void UFGSaveGameManager::LoadLevel(FEvent* Event)
{
	UGameplayStatics::OpenLevel(this, FName("PersistentLevel"));
	while (Event->Wait(1)) { LOG("Loading Level"); }
	Event->Trigger();
}

void UFGSaveGameManager::LoadGame(FString SaveName)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		if (UGameplayStatics::GetCurrentLevelName(this) != FString("PersistentLevel"))
		{
			FScopedEvent LoadingEvent;
			LoadLevel(LoadingEvent.Get());
		}
		TArray<uint8> SaveData;
		UGameplayStatics::LoadDataFromSlot(SaveData, SaveName, 0);
		UFGSaveGame* LoadedGame = Cast<UFGSaveGame>(UGameplayStatics::LoadGameFromMemory(SaveData));
		LoadedGame->Load();
	}
}

UFGSaveGameManager* UFGSaveGameManager::Get()
{
	static TWeakObjectPtr<UFGSaveGameManager> Instance;
	if (Instance.IsValid()) { return Instance.Get(); }

	Instance = NewObject<UFGSaveGameManager>();
	Instance->AddToRoot();
	if (Instance.IsValid()) { Instance->AddToRoot(); }

	return Instance.Get();
}

void UFGSaveGameManager::ReloadPuzzles()
{
	for (auto Puzzle : CompletedPuzzles)
	{
		for (auto* Man : PuzzleManagers)
		{
			if (Man != nullptr) { if (Man->GetFName() == Puzzle) { Man->SetPuzzleFinished(); } }
		}
	}
}

void UFGSaveGameManager::OnPuzzleComplete(AFGPuzzleManager* PuzzleManager)
{
	Get()->CompletedPuzzles.Add(PuzzleManager->GetFName());
}
