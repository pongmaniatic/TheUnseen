// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GAME/Characters/FGCharacter.h"

#include "GAME/Characters/FGPlayerController.h"

#include "GameFramework/Actor.h"

#include "FGMultiTool.generated.h"

class UTimelineComponent;
class UStaticMeshComponent;
class USceneComponent;
class USkeletalMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class GAME_API AFGMultiTool : public AActor
{
	GENERATED_BODY()

public:
	AFGMultiTool();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	UStaticMeshComponent* Base;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	UStaticMeshComponent* ChargeBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	UStaticMeshComponent* TopScreen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	UStaticMeshComponent* ScreenScreen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	UStaticMeshComponent* ScreenFrame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	USkeletalMeshComponent* Antenna;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	UStaticMeshComponent* ScanMeshSmall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	UTimelineComponent* TimelineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Position)
	USceneComponent* MultitoolEnd;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	FRotator ScreenOriginRotation = FRotator(0.000000, -90.000000, 0.000000);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	FRotator AntennaOriginRotation = FRotator(0.000000, 0.000000, 0.000000);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	FRotator AntennaTargetRotation = FRotator(0.000000, 0.000000, 0.000000);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	FRotator ScreenTargetRotation = FRotator(0.000000, 0.000000, 0.000000);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MTool)
	float RotationSpeed = 2.f;

public:
	UPROPERTY(VisibleInstanceOnly)
	bool bIsToolActive = true;
	 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interact)
	bool bHoldInteract = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Switch)
	float switchSpeedScreen = 0.1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Switch)
	float switchSpeedAntenna = 0.3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interact)
	bool bHoldInteractRight = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interact)
	FVector LastHitPos;

	virtual void Tick(float DeltaTime) override;

	void AnimateToolSwitch() const;

	void Interact(bool bIsLeft);
	UFUNCTION(BlueprintImplementableEvent, Category=Interact)
	void BP_OnSwitchFunction();
UFUNCTION()
	void SwitchFunction();

	void TraceForward();

	void TraceForwardMultitool();

	void InteractEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnInteract();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_InteractHeldTick();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnInteractEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Unfocused();
	
	UPROPERTY(Transient)
	AActor* FocusedActor;

	UPROPERTY(Transient)
	AFGCharacter* Player;

	UPROPERTY(Transient)
	APlayerController* PlayerController;

	UPROPERTY(VisibleInstanceOnly)
	FVector PlayerMeshStart;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interact)
	int mooltitoolAnimationSpeed;
};
