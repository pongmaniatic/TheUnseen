// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameState.h"

#include "FGGameState.generated.h"

class UFGSaveGameManager;
class UFGSensingManager;
/**
 *
 */
UCLASS()
class GAME_API AFGGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	virtual void PreInitializeComponents() override;
	virtual void ReceivedGameModeClass() override;

private:
	UPROPERTY(VisibleAnywhere)
	UFGSensingManager* SensingManager;

	UPROPERTY(VisibleAnywhere)
	UFGSaveGameManager* SaveGameManager;
};
