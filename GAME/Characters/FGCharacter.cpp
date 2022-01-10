#include "FGCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/FGMultiTool.h"
#include "Components/FGRadarTrackerComponent.h"
#include "Components/InputComponent.h"

#include "Engine/SkeletalMeshSocket.h"

#include "GAME/GAME.h"
#include "GAME/Managers/FGSensingManager.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

AFGCharacter::AFGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	MovementPtr = GetCharacterMovement();

	ACharacter::GetMovementComponent()->SetUpdatedComponent(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

	CameraComponent->SetupAttachment(GetCapsuleComponent());

	CameraComponent->SetRelativeLocation(FVector(-39.56f, 0, 64.f));

	CameraComponent->bUsePawnControlRotation = true;

	RadarTrackerComponent = CreateDefaultSubobject<UFGRadarTrackerComponent>(TEXT("ScannerTrackingComponent"));

	MovementPtr->MaxWalkSpeed = 500.0f;

	//Connecting the arm to the camera;

	GetMesh()->SetupAttachment(CameraComponent);

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	UFGSensingManager::Get()->Player = this;
}

void AFGCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) { Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit); }

void AFGCharacter::BeginPlay()
{
	Super::BeginPlay();
	UFGSensingManager::Get()->Player = this;
	//Movement Setup

	CurrentSpeed = MinSpeed;

	RadarTrackerComponent->SensingManager = UFGSensingManager::Get();

	RadarTrackerComponent->SensingManager->Player = this;

	//Handle Death

	OnGotCaughtByMonster.AddDynamic(this, &AFGCharacter::HandleOnGotCaughtByMonster);

	OnEscapedMonster.AddDynamic(this, &AFGCharacter::HandleOnEscaped);

	RadarTrackerComponent->SensingManager->OnMonsterHasLineOfSight.AddDynamic(this, &AFGCharacter::HandleMonsterHasLineOfSight);

	RadarTrackerComponent->SensingManager->OnMonsterHasLineOfSight.AddDynamic(this, &AFGCharacter::HandleMonsterLostLineOfSight);

	MultiTool = GetWorld()->SpawnActor<AFGMultiTool>(MultiToolBPClass);

	if (GetMesh()->DoesSocketExist(FName("HANDSOCKET")))
	{
		//MultiTool->AddToRoot();

		USkeletalMeshSocket const* Socket = GetMesh()->GetSocketByName(FName("HANDSOCKET"));

		Socket->AttachActor(MultiTool, GetMesh());

		// }
	}
}

void AFGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("SwitchTool", IE_Released, this, &AFGCharacter::SwitchTool);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFGCharacter::InteractStart);

	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AFGCharacter::InteractEnd);

	PlayerInputComponent->BindAction("InteractRight", IE_Pressed, this, &AFGCharacter::InteractRightStart);

	PlayerInputComponent->BindAction("InteractRight", IE_Released, this, &AFGCharacter::InteractEnd);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFGCharacter::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &AFGCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &AFGCharacter::TurnAtRate);

	PlayerInputComponent->BindAxis("LookUpRate", this, &AFGCharacter::LookUpAtRate);
}

void AFGCharacter::Tick(float DeltaTime)
{
	SCOPED_NAMED_EVENT(NAME_Actor, FColor::Red)

	Super::Tick(DeltaTime);

	// Is Walking and is grounded below;

	FVector InputVector = GetLastMovementInputVector();

	if (IsGrounded())
	{
		if (!InputVector.IsNearlyZero()) { bIsPlayerWalking = true; }

		else { bIsPlayerWalking = false; }
	}

	else { bIsPlayerWalking = false; }

	//Check if we are dying/Getting caught;

	if (bIsGettingCaught) { GettingCaught(DeltaTime); }
}

bool AFGCharacter::IsGrounded()
{
	FHitResult Hit;

	FVector TraceLine = GetActorLocation() + FVector::DownVector * 90.f;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);

	if (UKismetSystemLibrary::LineTraceSingle(this, TraceLine, TraceLine + FVector::DownVector * 20.f, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true))
	{
		if (Hit.GetActor() == this) { LOG("Hit itself"); }

		return true;
	}

	return false;
}

AFGMultiTool* AFGCharacter::GetMultitool()
{
	ensure(MultiTool!=nullptr);

	return MultiTool;
}

bool AFGCharacter::IsPlayerWalking() { return bIsPlayerWalking; }

void AFGCharacter::HandleMonsterLostLineOfSight_Implementation()
{
	CurrentSpeed = 500;

	LOG("PLAYER CURRENT SPEED: 500")
}

void AFGCharacter::HandleMonsterHasLineOfSight_Implementation()
{
	CurrentSpeed = 800;
	LOG("PLAYER CURRENT SPEED: 800")
}

void AFGCharacter::HandleOnEscaped_Implementation()
{
	bIsGettingCaught = false;

	LOG("ESCAPED")
}

void AFGCharacter::HandleOnGotCaughtByMonster_Implementation()
{
	bIsGettingCaught = true;

	LOG("IS GETTING CAUGHT")
}

void AFGCharacter::GettingCaught(float DeltaTime)
{
	GettingCaughtTimer += DeltaTime;

	if (GettingCaughtTimer >= GettingCaughtTime)
	{
		GettingCaughtTimer = 0.0f;

		bIsGettingCaught = false;

		OnGameOver.Broadcast();
	}
}

/**



 * INTERACTIONS BELOW;



 */

void AFGCharacter::InteractStart()
{
	if (MultiTool->bIsToolActive) { MultiTool->Interact(true); }

	else { RadarTrackerComponent->Scan(); }
}

void AFGCharacter::InteractEnd() { MultiTool->InteractEnd(); }

void AFGCharacter::InteractRightStart()
{
	if (MultiTool->bIsToolActive) { MultiTool->Interact(false); }

	else { RadarTrackerComponent->Scan(); }
}

void AFGCharacter::SwitchTool() { MultiTool->SwitchFunction(); }

/**



 *



 *  MOVEMENT BELOW



 */

void AFGCharacter::TurnAtRate(float Rate) { AddControllerYawInput(Rate * BaseTurnRate); }

void AFGCharacter::LookUpAtRate(float Rate) { AddControllerPitchInput(Rate * BaseLookUpRate); }

void AFGCharacter::MoveForward(float Value)
{
	MovementPtr->MaxWalkSpeed = CurrentSpeed;

	AddMovementInput(GetActorForwardVector(), Value);
}

void AFGCharacter::MoveRight(float Value)
{
	MovementPtr->MaxWalkSpeed = CurrentSpeed;

	AddMovementInput(CameraComponent->GetRightVector(), Value);
}
