// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGGameInstance.generated.h"


class UGameMapsSettings;


UCLASS()
class GAME_API UFGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFGGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Get FGGameInstance"))
	static UFGGameInstance* Get();

};
