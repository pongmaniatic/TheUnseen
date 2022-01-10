#include "FGEnemyBase.h"

#include "DrawDebugHelpers.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyAI/FGRadarTrackingTarget.h"

#include "GAME/Managers/FGSensingManager.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "Kismet/KismetSystemLibrary.h"

AFGEnemyBase::AFGEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = true;

	ScannerTargetComponent = CreateDefaultSubobject<UFGRadarTrackingTarget>(TEXT("ScannerTargetComponent"));

	SetActorEnableCollision(true);

}

void AFGEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	SensingManager = UFGSensingManager::Get();

	ensure(SensingManager != nullptr);
	SensingManager->Enemy = this;
	SensingManager->OnMonsterHasLineOfSight.AddDynamic(this, &AFGEnemyBase::HandleOnHasLineOfSight);
	SensingManager->OnMonsterLostLineOfSight.AddDynamic(this, &AFGEnemyBase::HandleOnLostLineOfSight);
	OnActorBeginOverlap.AddDynamic(this, &AFGEnemyBase::HandleOnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AFGEnemyBase::HandleOnEndOverlap);
}

float AFGEnemyBase::GetDistanceToPlayer()
{
	float Distance = GetDistanceTo(SensingManager->Player);
	return Distance;
}

bool AFGEnemyBase::LineOfSightTo(AActor* Other) { return GetController()->LineOfSightTo(Other, FVector::ZeroVector); }

void AFGEnemyBase::LineOfSightCheck(float DeltaTime)
{
	FVector Point;

	FRotator Rot;

	GetActorEyesViewPoint(Point, Rot);

	//if (IsPlayerInVisionRadius(Rot, Point))

	if (GetDistanceToPlayer() <= VisionDistance)
	{
		if (LineOfSightTo(SensingManager->Player))
		{
			if (bDebugDraw) { UKismetSystemLibrary::DrawDebugLine(GetWorld(), Point, SensingManager->Player->GetActorLocation(), FColor::Cyan); }

			if (!bHasLineOfSight) { SensingManager->OnMonsterHasLineOfSight.Broadcast(); }

			bHasLineOfSight = true;
		}

		else if (bHasLineOfSight)
		{
			SensingManager->OnMonsterHasLineOfSight.Broadcast();

			bHasLineOfSight = false;
		}
	}

	else
	{
		if (bHasLineOfSight) { SensingManager->OnMonsterLostLineOfSight.Broadcast(); }

		bHasLineOfSight = false;
	}

	if (bDebugDraw)
	{
		FString msg = "Line of Sight: ";

		msg += bHasLineOfSight ? "True" : "False";

		GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaTime, FColor::Cyan, msg);
	}
}

void AFGEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (bDebugDraw) { LineOfSightCheck(DeltaTime); }
	if (bHasRequestedLocation)
	{
		Move(DeltaTime);
		if (bDebugDraw)
		{
			DrawDebugString(GetWorld(), RequestedLocation, FString("ENEMY TARGET LOCATION"));
			DrawDebugLine(GetWorld(), GetActorLocation(), RequestedLocation, FColor::Blue, false, DeltaTime, 0, 4.f);
		}
	}
}

void AFGEnemyBase::MoveTowards(FVector Location)
{
	RequestedLocation = Location;
	bHasRequestedLocation = true;
}

void AFGEnemyBase::Move(float DeltaTime)
{
	if (bHasRequestedLocation)
	{
		FVector DeltaDirection = (RequestedLocation - GetActorLocation()).GetSafeNormal();
		float ScaleValue = FMath::Clamp(AggressionLevel, 0.f, 1.f);
		FRotator DeltaRotation = GetControlRotation() + DeltaDirection.Rotation();

		AddMovementInput(DeltaDirection, ScaleValue);
		Controller->SetControlRotation(DeltaRotation);

		if (GetActorLocation().Equals(RequestedLocation))
		{
			bHasRequestedLocation = false;
			RequestedLocation = GetActorLocation();
		}
	}
}

bool AFGEnemyBase::IsPlayerInVisionRadius(FRotator EyeRot, FVector EyePoint)
{
	//TODO Make Angle and Distance Minimum adjustable for designers.

	//Above needs not to be done since we are not using the radius, monster sees in 360;

	FVector Direction = EyeRot.Vector();

	FVector Origin = EyePoint;

	float Angle = 45;

	float DistanceMinimum = 1000.f;

	const float DistanceMinimumSq = FMath::Square(DistanceMinimum);

	FVector PointToTest = SensingManager->Player->GetActorLocation();

	if (bDebugDraw)
	{
		FVector Right = Direction.RotateAngleAxis(Angle, FVector::UpVector);

		FVector Left = Direction.RotateAngleAxis(-Angle, FVector::UpVector);

		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * DistanceMinimumSq, FLinearColor::Red);

		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * DistanceMinimumSq, FLinearColor::Green);
	}

	if (FVector::DistSquared(Origin, PointToTest) > DistanceMinimumSq) { return false; }

	const FVector DirectionToTarget = (PointToTest - Origin).GetSafeNormal();

	const float Rad = FMath::Cos(FMath::DegreesToRadians(Angle));

	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	if (Dot >= Rad) { return true; }

	return false;
}

// void AFGEnemyBase::HandleOnHitEvent(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)

// {

// 	AFGCharacter* Player = Cast<AFGCharacter>(OtherActor);

// 	if (!Player) { return; }

// 	if (Player->bIsGettingCaught) { return; }

// 	Player->OnGotCaughtByMonster.Broadcast();

// }

void AFGEnemyBase::HandleOnBeginOverlap(AActor* SelfActor, AActor* OverlapActor)
{
	AFGCharacter* PlayerChar = Cast<AFGCharacter>(OverlapActor);

	if (!PlayerChar) { return; }

	PlayerChar->OnGotCaughtByMonster.Broadcast();
}

void AFGEnemyBase::HandleOnEndOverlap(AActor* SelfActor, AActor* OverlapActor)
{
	AFGCharacter* PlayerChar = Cast<AFGCharacter>(OverlapActor);

	if (!PlayerChar) { return; }

	PlayerChar->OnEscapedMonster.Broadcast();
}

void AFGEnemyBase::HandleOnLostLineOfSight() {}

void AFGEnemyBase::HandleOnHasLineOfSight() {}
