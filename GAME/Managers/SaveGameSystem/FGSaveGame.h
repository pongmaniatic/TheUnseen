// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/SaveGame.h"

#include "FGSaveGame.generated.h"

class AInteractableBase;
class AGameStateBase;
class UFGGameInstance;
class AGAMEGameModeBase;
/**
 *
 */
UCLASS()
class GAME_API UFGSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UFGSaveGame();
	bool Init(FString _SaveName);
	void Load();
	bool GetCompletedPuzzles();

	UPROPERTY(SaveGame)
	FString SaveName;
	UPROPERTY(SaveGame)
	FString SaveTime;
	UPROPERTY(SaveGame)
	FTransform PlayerTransform;
	UPROPERTY(SaveGame)
	FTransform MonsterTransform;
	UPROPERTY(SaveGame)
	TArray<TSoftObjectPtr<AInteractableBase>> Interactables;
	UPROPERTY(SaveGame)
	TArray<FName> CompletedPuzzles;
};
