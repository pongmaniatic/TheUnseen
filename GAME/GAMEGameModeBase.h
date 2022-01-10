// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "GAMEGameModeBase.generated.h"

class AFGEnemyBase;
class AFGCharacter;
class UFGSensingManager;

UCLASS(Blueprintable, BlueprintType)
class GAME_API AGAMEGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGAMEGameModeBase();


};
