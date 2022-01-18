// Fill out your copyright notice in the Description page of Project Settings.

#include "FGMultiTool.h"

#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "GAME/Environment/Interactable/InteractableBase.h"

// Sets default values

AFGMultiTool::AFGMultiTool()
{
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));

	SetRootComponent(Base); 

	ChargeBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChargeBar"));
	ChargeBar->SetupAttachment(Base);
	TopScreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopScreen"));
	TopScreen->SetupAttachment(Base);
	ScreenFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenFrame"));
	ScreenFrame->SetupAttachment(Base);
	ScreenScreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenScreen"));
	ScreenScreen->SetupAttachment(ScreenFrame);
	Antenna = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Antenna"));
	Antenna->SetupAttachment(Base);
	ScanMeshSmall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScanMeshSmall"));

	ScanMeshSmall->SetupAttachment(Base);

	MultitoolEnd = CreateDefaultSubobject<USceneComponent>(TEXT("MultitoolEnd"));

	MultitoolEnd->SetupAttachment(Base);



	PrimaryActorTick.bCanEverTick = true;
}

void AFGMultiTool::BeginPlay() 
{ 
	Super::BeginPlay(); 
	PlayerController = GetWorld()->GetFirstPlayerController();
	//AActor* playerActor = GetOwner();
	//Player = Cast<AFGCharacter>(playerActor);

	
}

void AFGMultiTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//TraceForwardMultitool();
	if (bHoldInteract)
	{
		TraceForward();
		if (FocusedActor)
		{
			IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
			AInteractableBase* ActorBase = Cast<AInteractableBase>(FocusedActor);
			if (Interface)
			{
				if (FVector::Dist(GetActorLocation(), LastHitPos) < ActorBase->maxInteractableDistance)
				{
					BP_InteractHeldTick();
					Interface->Execute_Held(FocusedActor, this);
				}
			}
		}
	}
	else if (bHoldInteractRight)
	{
		TraceForward();
		if (FocusedActor)
		{
			IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
			AInteractableBase* ActorBase = Cast<AInteractableBase>(FocusedActor);
			if (Interface)
			{
				if (FVector::Dist(GetActorLocation(), LastHitPos) < ActorBase->maxInteractableDistance)
				{
					BP_InteractHeldTick();
					Interface->Execute_HeldRight(FocusedActor, this);
				}
			}
		}
	}

	AnimateToolSwitch();

}

void AFGMultiTool::AnimateToolSwitch() const
{
	if (!bIsToolActive)
	{
		if (Antenna->GetRelativeRotation() != AntennaTargetRotation)
		{
			Antenna->SetRelativeRotation(FMath::Lerp(Antenna->GetRelativeRotation(), AntennaTargetRotation, switchSpeedAntenna));
		}
		if (ScreenFrame->GetRelativeRotation() != ScreenTargetRotation)
		{
			ScreenFrame->SetRelativeRotation(FMath::Lerp(ScreenFrame->GetRelativeRotation(), ScreenTargetRotation, switchSpeedScreen));
		}
	}

	else
	{
		if (Antenna->GetRelativeRotation() != AntennaOriginRotation)
		{
			Antenna->SetRelativeRotation(FMath::Lerp(Antenna->GetRelativeRotation(), AntennaOriginRotation, switchSpeedAntenna));
		}
		if (ScreenFrame->GetRelativeRotation() != ScreenOriginRotation)
		{
			ScreenFrame->SetRelativeRotation(FMath::Lerp(ScreenFrame->GetRelativeRotation(), ScreenOriginRotation, switchSpeedScreen));
		}
	}
}

void AFGMultiTool::Interact(bool bIsLeft)
{
	if (bIsToolActive)
	{
		if (bIsLeft) { bHoldInteract = true; }

		else { bHoldInteractRight = true; }

		TraceForward();

		if (FocusedActor)
		{
			IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);

			AInteractableBase* ActorBase = Cast<AInteractableBase>(FocusedActor);

			if (Interface)

				if (FVector::Dist(GetActorLocation(), LastHitPos) < ActorBase->maxInteractableDistance)
				{
					BP_OnInteract();

					if (bHoldInteract) { Interface->Execute_OnInteract(FocusedActor, this); }

					else { Interface->Execute_OnInteractRight(FocusedActor, this); }
				}
		}
	}
}

void AFGMultiTool::SwitchFunction()

{
	bIsToolActive = !bIsToolActive;

	AnimateToolSwitch();

	BP_OnSwitchFunction();
}

void AFGMultiTool::TraceForward()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	PlayerController->GetPlayerViewPoint(OUT PlayerViewPointLocation,
	                                                           OUT PlayerViewPointRotation);
	FVector startPoint = PlayerViewPointLocation ;
	FVector endPoint = startPoint + PlayerViewPointRotation.Vector() * 2000;
	FCollisionQueryParams CollisionParams;
	FHitResult OutHit;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, startPoint, endPoint, ECC_Visibility, CollisionParams))
	{
		LastHitPos = OutHit.ImpactPoint;
		AActor* Interactable = OutHit.GetActor();
		IInteractableInterface* Interface = Cast<IInteractableInterface>(Interactable);

		if (Interface)
		{
			if (Interactable != FocusedActor) 
			{ 
				FocusedActor = Interactable;
				AInteractableBase* ActorBase = Cast<AInteractableBase>(FocusedActor);
				mooltitoolAnimationSpeed = ActorBase->mooltitoolAnimationSpeed;
				mooltitoolAnimationSpeed = ActorBase->mooltitoolAnimationSpeed;
			}
		}
		else
		{
			if (FocusedActor)
			{
				BP_Unfocused();
				Interface->Execute_Released(FocusedActor, this);
				FocusedActor = nullptr;
				mooltitoolAnimationSpeed = 0;
			}
		}
	}
}

void AFGMultiTool::TraceForwardMultitool()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	PlayerController->GetPlayerViewPoint(OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	FVector startPoint = PlayerViewPointLocation + FVector(0, 0, 0);
	FVector endPoint = startPoint + PlayerViewPointRotation.Vector() * 2000;
	FCollisionQueryParams CollisionParams;
	FHitResult OutHit;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, startPoint, endPoint, ECC_Visibility, CollisionParams))
	{
		LastHitPos = OutHit.ImpactPoint;
		AActor* Interactable = OutHit.GetActor();
		IInteractableInterface* Interface = Cast<IInteractableInterface>(Interactable);
		float distanceToHitPoint = FVector::Dist(LastHitPos, startPoint);
		if (distanceToHitPoint <= 120)
		{

			distanceToHitPoint = 120 - distanceToHitPoint;
			FString TheFloatStr = FString::SanitizeFloat(distanceToHitPoint);
			UE_LOG(LogTemp, Warning, TEXT("distance to hit: %s"), *TheFloatStr);

			Player->SetActorRelativeLocation(PlayerMeshStart + FVector(0, 0, distanceToHitPoint));

		}
	}



}

void AFGMultiTool::InteractEnd()

{
	bHoldInteract = false;

	bHoldInteractRight = false;

	BP_OnInteractEnd();

	TraceForward();

	if (FocusedActor)
	{
		IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);

		AInteractableBase* ActorBase = Cast<AInteractableBase>(FocusedActor);

		if (Interface)

			if (FVector::Dist(GetActorLocation(), LastHitPos) < ActorBase->maxInteractableDistance)

				Interface->Execute_Released(FocusedActor, this);
	}
}
