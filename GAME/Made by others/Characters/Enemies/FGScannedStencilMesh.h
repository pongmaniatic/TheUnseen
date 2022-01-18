// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GAME/Characters/Components/FGScannedObjectInterface.h"

#include "GameFramework/Actor.h"

#include "FGScannedStencilMesh.generated.h"

UCLASS(Blueprintable, BlueprintType)
class GAME_API AFGScannedStencilMesh : public AActor, public IFGScannedObjectInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFGScannedStencilMesh();
	AFGScannedStencilMesh(float _InitialLifeSpan);

	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void LifeSpanExpired() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StencilValue = 2.f;
	UPROPERTY()
	bool bIsActivated = false;
	virtual void Activate() override;
	virtual void Deactivate() override;
};
