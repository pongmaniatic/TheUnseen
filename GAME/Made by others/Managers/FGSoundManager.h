// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FGSoundManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFGOnPlayFootsteps);

UCLASS(Blueprintable, BlueprintType)
class GAME_API AFGSoundManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFGSoundManager();

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Get SoundManager"))
	static AFGSoundManager* Get();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
