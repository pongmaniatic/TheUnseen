// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FGRadarTrackerComponent.generated.h"

class UFGSensingManager;
class AFGEnemyBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAME_API UFGRadarTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFGRadarTrackerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	UFGSensingManager* SensingManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Scanner)
	UMaterialParameterCollection* ScannerParameterCollection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Scanner)
	float ScanSpeed = 0.1f;

	/**
	 * @brief Current Radius. Multiplies with Max Distance (set in the material Parameter Collection);
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=Scanner)
	float Radius = 0.0f;

	bool bScanActive = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Scanner)
	float ScannerTimeLimit = 10.f;

	float ScannerTimer = 0.0f;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void Scan();
};
