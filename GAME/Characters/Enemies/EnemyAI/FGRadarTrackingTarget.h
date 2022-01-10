// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "FGRadarTrackingTarget.generated.h"

class AFGScannedStencilMesh;
class USphereComponent;
class UFGSensingManager;
class FFGOnScannerRequest;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class GAME_API UFGRadarTrackingTarget : public UActorComponent
{
private:
	GENERATED_BODY()

public:
	UFGRadarTrackingTarget();
protected:
	virtual void BeginPlay() override;

public:
	bool bIsVisibleOnScanner = false;

	UPROPERTY()
	UFGSensingManager* SensingManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Scanner)
	TArray<UStaticMeshComponent*> ScannedMeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Scanner)
	TSubclassOf<AFGScannedStencilMesh> StencilMeshClass;
	UPROPERTY()
	bool bSpawnStencilMesh = false;
	UPROPERTY()
	bool bIsSkeletalMesh = false;
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Scanner)
	float ScannedMeshLifetime = 2.f;
	/**
	 * @brief this value corresponds to the type of object that is visible on the scanner. Range is 0-255 where 0 is not visible.
	 * 1 is reserved for the enemy
	 * 2 is the standard for interactable objects.
	 * if more needs to be added it needs to be set in the M_Scanner shader.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Scanner)
	float StencilValue = 2.f;
	UPROPERTY()
	float ScannedMeshLifeTimer = 0.0f;
	UFUNCTION(BlueprintInternalUseOnly)
	void HandleScannerRequest();
	UFUNCTION()
	void SpawnStencilMesh();
	UFUNCTION()
	void DeactivateVisibleOnScanner();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
