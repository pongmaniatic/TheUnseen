// Fill out your copyright notice in the Description page of Project Settings.

#include "FGSensingManager.h"

#include "GAME/Characters/Enemies/FGEnemyBase.h"

#include "Kismet/GameplayStatics.h"

UFGSensingManager::UFGSensingManager() {}

UFGSensingManager* UFGSensingManager::Get()
{
	static TWeakObjectPtr<UFGSensingManager> Instance;

	if (Instance.IsValid()) { return Instance.Get(); }

	Instance = NewObject<UFGSensingManager>();

	if (Instance.IsValid()) { Instance->AddToRoot(); }

	return Instance.Get();
}

void UFGSensingManager::ScannerRequest() { OnScannerRequest.Broadcast(); }
