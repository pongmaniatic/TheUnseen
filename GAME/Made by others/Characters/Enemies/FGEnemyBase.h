#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BlackboardData.h"

#include "GameFramework/Character.h"
#include "GameFramework/DefaultPawn.h"

#include "FGEnemyBase.generated.h"

struct FBlackboardEntry;
class UBlackboardData;
class UBlackboardKeyType_Enum;
class UBlackboardKeyType_Class;
class UBlackboardComponent;
class UBlackboardKeyType_Vector;
class UFGSensingManager;
class UFGRadarTrackingTarget;
class UFGEnemyStateMachine;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFGOnHasLineOfSight, FVector, PlayerPos);

UCLASS(Blueprintable, BlueprintType)

class GAME_API AFGEnemyBase : public ADefaultPawn
{
	GENERATED_BODY()

public:
	AFGEnemyBase();

	UPROPERTY()
	FVector RequestedLocation;
	UPROPERTY()
	bool bHasRequestedLocation;
	/**
	 * @brief Affects Enemy move speed and how long it will wait in IDLE after not finding the player;
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	float AggressionLevel = 1.0f;

protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void Move(float DeltaTime);
	UFUNCTION()
	bool LineOfSightTo(AActor* Other);
	UFUNCTION()
	void LineOfSightCheck(float DeltaTime);
	UFUNCTION()
	bool IsPlayerInVisionRadius(FRotator EyeRot, FVector EyePoint);
	UFUNCTION()
	void HandleOnBeginOverlap(AActor* SelfActor, AActor* OverlapActor);
	UFUNCTION()
	void HandleOnEndOverlap(AActor* SelfActor, AActor* OverlapActor);
	UFUNCTION(BlueprintInternalUseOnly)
	void HandleOnLostLineOfSight();
	UFUNCTION(BlueprintInternalUseOnly)
	void HandleOnHasLineOfSight();
public:
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, meta=(DisplayName="MoveTo"))
	void MoveTowards(FVector Location);

	UFUNCTION(BlueprintCallable, Category=AI, meta=(DisplayName="GetDistanceToPlayer"))
	float GetDistanceToPlayer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Debug)
	bool bDebugDraw = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=AI)
	bool bHasLineOfSight = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= AI)
	UFGSensingManager* SensingManager;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=AI)
	UFGRadarTrackingTarget* ScannerTargetComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=AI)
	float VisionDistance = 1000.f;
};
