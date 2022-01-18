// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/Character.h"

#include "FGCharacter.generated.h"

class UFGRadarTrackerComponent;

class USkeletalMeshComponent;

class UCameraComponent;

class UInputComponent;

class AFGMultiTool;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFGOnCustomEventsDelegate);

UCLASS(BlueprintType, Blueprintable)

class GAME_API AFGCharacter : public ACharacter

{
	GENERATED_BODY()

public:
	AFGCharacter();

	UPROPERTY(VisibleDefaultsOnly)
	AFGMultiTool* MultiTool;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFGMultiTool> MultiToolBPClass;

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Get Multitool"))
	AFGMultiTool* GetMultitool();

	UFUNCTION(BlueprintCallable)
	bool IsPlayerWalking();

	UPROPERTY(VisibleInstanceOnly)
	bool bIsPlayerWalking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* MovementPtr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category=Movement)
	float BaseTurnRate = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category=Movement)
	float BaseLookUpRate = 0.6f;

	UFUNCTION(BlueprintNativeEvent)
	void HandleMonsterLostLineOfSight();
	void HandleMonsterLostLineOfSight_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void HandleMonsterHasLineOfSight();
	void HandleMonsterHasLineOfSight_Implementation();

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category=Death, Transient)
	FFGOnCustomEventsDelegate OnGotCaughtByMonster;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category=Death, Transient)
	FFGOnCustomEventsDelegate OnEscapedMonster;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category=Death, Transient)
	FFGOnCustomEventsDelegate OnGameOver;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGameOver();

	UFUNCTION(BlueprintNativeEvent, Category=Death)
	void HandleOnEscaped();

	UFUNCTION(BlueprintNativeEvent, Category=Death)
	void HandleOnGotCaughtByMonster();

	UPROPERTY(BlueprintReadOnly, Category=Death)
	bool bIsGettingCaught = false;

	UFUNCTION()
	void GettingCaught(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, Category=Death)
	float GettingCaughtTimer = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Death)
	float GettingCaughtTime = 1.f;

protected:
	virtual void BeginPlay() override;

	bool IsGrounded();

	//Input and controls

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void InteractStart();
	void InteractEnd();
	void InteractRightStart();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	//MultiTool Actions;

	void SwitchTool();

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	int MinSpeed = 500;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	int CurrentSpeed = 500;

	UFUNCTION()
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UFGRadarTrackerComponent* RadarTrackerComponent;
};
