// Fill out your copyright notice in the Description page of Project Settings.

#include "FGPuzzleManager.h"

#include "FGGameInstance.h"

#include "SaveGameSystem/FGSaveGameManager.h"

// Sets default values
AFGPuzzleManager::AFGPuzzleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFGPuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	UFGSaveGameManager::Get()->PuzzleManagers.Add(this);
}

// Called every frame
void AFGPuzzleManager::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
