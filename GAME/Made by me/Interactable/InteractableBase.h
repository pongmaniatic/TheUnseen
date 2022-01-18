// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractableBase.generated.h"

class UFGRadarTrackingTarget;


UCLASS(Blueprintable, BlueprintType)
class GAME_API AInteractableBase : public AActor, public IInteractableInterface
{
private:
	GENERATED_BODY()

public:
	AInteractableBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFGRadarTrackingTarget* ScannerTrackingTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interactable)
	int maxInteractableDistance = 200;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interactable)
	int mooltitoolAnimationSpeed = 25;



protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Puzzle)
	bool taskFinished;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Puzzle)
	FTransform finishedState;

	virtual void Tick(float DeltaTime) override;
};
