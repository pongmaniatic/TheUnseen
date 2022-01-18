#pragma once
#include "CoreMinimal.h"

#include "EnemyState.generated.h"


UENUM(BlueprintType, Blueprintable, Category=AI, Meta=(ScriptName="EnemyState"))
enum class EEnemyState : uint8
{

	IDLE UMETA(DisplayName="IDLE"),
	CHASING UMETA(DisplayName="CHASING"),
	SEARCHING UMETA(DisplayName="SEARCHING"),
	HOLDLOGIC UMETA(DisplayName="HOLDINGLOGIC"),
};
