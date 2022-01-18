// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FGScannedObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFGScannedObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class GAME_API IFGScannedObjectInterface
{
	GENERATED_BODY()
public:
	virtual void Activate();
	virtual void Deactivate();
};
