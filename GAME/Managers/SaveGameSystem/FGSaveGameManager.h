// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GAME/Managers/FGGameState.h"

#include "Kismet/GameplayStatics.h"

#include "UObject/Object.h"

#include "FGSaveGameManager.generated.h"

/**

 *

 */

class AFGPuzzleManager;
class UFGGameInstance;
class UFGSaveGame;
class UGameplayStatics;
class USaveGame;
UCLASS()

class GAME_API UFGSaveGameManager : public UObject
{
	GENERATED_BODY()
public:
	UFGSaveGameManager();

	UFUNCTION(BlueprintCallable, meta=(DisplayName="SaveGame"))
	void SaveGame(FString SaveName);

	UPROPERTY()
	AFGGameState* GameState;

	//Game State Stuff:
	UPROPERTY(EditAnywhere)
	TArray<FName> CompletedPuzzles;

	UPROPERTY(VisibleAnywhere)
	TArray<AFGPuzzleManager*> PuzzleManagers;

	UFUNCTION(BlueprintCallable)
	void DeleteSaveGame(FString SaveName);
	void LoadLevel(FEvent* Event);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="LoadGame"))
	void LoadGame(FString SaveName);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="GetSaveGameManager"))
	static UFGSaveGameManager* Get();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<USaveGame> SaveGameClass;

	UFUNCTION()
	void ReloadPuzzles();

	UFUNCTION(BlueprintCallable, meta=(DisplayName="On Puzzle Complete"))
	static void OnPuzzleComplete(AFGPuzzleManager* PuzzleManager);
};
