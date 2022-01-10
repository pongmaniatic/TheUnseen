// Fill out your copyright notice in the Description page of Project Settings.

#include "FGPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values

AFGPlayerController::AFGPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned

void AFGPlayerController::BeginPlay() { Super::BeginPlay(); }

// Called every frame

void AFGPlayerController::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
