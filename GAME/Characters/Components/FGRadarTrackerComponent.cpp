// Fill out your copyright notice in the Description page of Project Settings.

#include "FGRadarTrackerComponent.h"

#include "GAME/Managers/FGSensingManager.h"

#include "Kismet/KismetMaterialLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

UFGRadarTrackerComponent::UFGRadarTrackerComponent() { PrimaryComponentTick.bCanEverTick = true; }

void UFGRadarTrackerComponent::BeginPlay()
{
	Super::BeginPlay();

	SensingManager = UFGSensingManager::Get();

	SensingManager->PlayerScannerComponent = this;

	ScanSpeed = UKismetMaterialLibrary::GetScalarParameterValue(this, ScannerParameterCollection,
	                                                            FName("ScannerSpeed"));

}

// Called every frame

void UFGRadarTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType,

                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
ScannerTimer -= DeltaTime;
	if (bScanActive)
	{
		Radius += FMath::Lerp(0.f, 1.f, ScanSpeed * DeltaTime);

		if (Radius >= 1.f)
		{
			bScanActive = false;

			Radius = 0.0f;
		}

		UKismetMaterialLibrary::SetScalarParameterValue(this, ScannerParameterCollection, FName("Radius"), Radius);
	}
}

void UFGRadarTrackerComponent::Scan()
{
	if (!bScanActive)
	{
		if (ScannerTimer <= 0.f)
		{
			bScanActive = true;
			SensingManager->OnScannerRequest.Broadcast();
			ScannerTimer = ScannerTimeLimit;
		}
	}
}
