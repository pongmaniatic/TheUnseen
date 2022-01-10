// Fill out your copyright notice in the Description page of Project Settings.

#include "FGGameState.h"

#include "FGSensingManager.h"

#include "SaveGameSystem/FGSaveGameManager.h"
void AFGGameState::BeginPlay() { Super::BeginPlay(); }

void AFGGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	SensingManager = UFGSensingManager::Get();
	SaveGameManager = UFGSaveGameManager::Get();
}

void AFGGameState::ReceivedGameModeClass() { Super::ReceivedGameModeClass(); }
