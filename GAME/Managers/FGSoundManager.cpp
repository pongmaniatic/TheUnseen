// Fill out your copyright notice in the Description page of Project Settings.

#include "FGSoundManager.h"

#include "FGGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFGSoundManager::AFGSoundManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AFGSoundManager* AFGSoundManager::Get()
{
	static TWeakObjectPtr<AFGSoundManager> Instance;
	if (Instance.IsValid()) return Instance.Get();

	Instance = NewObject<AFGSoundManager>();

	if (Instance.IsValid()) Instance->AddToRoot();

	return Instance.Get();
}

// Called when the game starts or when spawned
void AFGSoundManager::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AFGSoundManager::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
