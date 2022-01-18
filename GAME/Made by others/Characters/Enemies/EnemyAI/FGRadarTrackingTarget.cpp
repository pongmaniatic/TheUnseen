#include "FGRadarTrackingTarget.h"

#include "GAME/Characters/Enemies/FGScannedStencilMesh.h"

#include "GAME/Managers/FGSensingManager.h"

UFGRadarTrackingTarget::UFGRadarTrackingTarget()

{
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetAutoActivate(true);
}

void UFGRadarTrackingTarget::BeginPlay()

{
	Super::BeginPlay();

	SensingManager = UFGSensingManager::Get();

	ensure(SensingManager!=nullptr);

	SensingManager->RadarTrackingTargets.Add(this);

	SensingManager->OnScannerRequest.AddDynamic(this, &UFGRadarTrackingTarget::HandleScannerRequest);

	bIsVisibleOnScanner = false;

	if (StencilMeshClass == nullptr)
	{
		TSet<UActorComponent*> ActorComp = GetOwner()->GetComponents();

		if (ActorComp.Num() > 0)
		{
			for (auto Comp : ActorComp)
			{
				UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Comp);

				if (!Mesh)
				{
					USkeletalMeshComponent* SkelMesh = Cast<USkeletalMeshComponent>(Comp);

					if (!SkelMesh)

						continue;

					bIsSkeletalMesh = true;

					SkelMesh->bRenderStatic = true;

					SkelMesh->bRenderCustomDepth = false;

					SkelMesh->CustomDepthStencilValue = 0;

					SkeletalMeshComponent = SkelMesh;

					continue;
				}

				if (Mesh->GetStaticMesh() == nullptr) { continue; }

				ScannedMeshComponents.Add(Mesh);

				Mesh->bRenderCustomDepth = false;

				Mesh->MarkRenderStateDirty();
			}
		}

		for (auto Comp : GetOwner()->BlueprintCreatedComponents)
		{
			UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Comp);

			if (!Mesh) { continue; }

			ScannedMeshComponents.Add(Mesh);

			Mesh->bRenderCustomDepth = false;

			Mesh->MarkRenderStateDirty();
		}

		bSpawnStencilMesh = false;
	}

	else { bSpawnStencilMesh = true; }
}

void UFGRadarTrackingTarget::HandleScannerRequest()

{
	bIsVisibleOnScanner = true;

	if (bSpawnStencilMesh) { SpawnStencilMesh(); }

	else if (bIsSkeletalMesh)
	{
		SkeletalMeshComponent->bRenderStatic = true;

		SkeletalMeshComponent->bRenderInDepthPass = true;

		SkeletalMeshComponent->bRenderCustomDepth = true;

		SkeletalMeshComponent->CustomDepthStencilValue = StencilValue;

		SkeletalMeshComponent->MarkRenderStateDirty();
	}

	else
	{
		if (ScannedMeshComponents.Num() > 0)
		{
			for (auto Comp : ScannedMeshComponents)
			{
				if (Comp == nullptr) { continue; }

				Comp->bRenderInDepthPass = true;

				Comp->bRenderCustomDepth = true;

				Comp->CustomDepthStencilValue = StencilValue;

				Comp->MarkRenderStateDirty();
			}
		}
	}
}

void UFGRadarTrackingTarget::SpawnStencilMesh()

{
	FVector Location = GetOwner()->GetActorLocation();

	Location.Z -= 120.f;

	FRotator Rotator = GetOwner()->GetActorRotation();

	Rotator.Add(0.f, -90.f, 0.f);

	AFGScannedStencilMesh* SpawnedMesh = GetWorld()->SpawnActor<AFGScannedStencilMesh>(

		StencilMeshClass,

		Location,

		Rotator);

	SpawnedMesh->Activate();
}

void UFGRadarTrackingTarget::DeactivateVisibleOnScanner()

{
	bIsVisibleOnScanner = false;

	if (!bSpawnStencilMesh)
	{
		if (bIsSkeletalMesh)
		{
			SkeletalMeshComponent->bRenderStatic = false;

			SkeletalMeshComponent->bRenderInDepthPass = false;

			SkeletalMeshComponent->bRenderCustomDepth = false;

			SkeletalMeshComponent->CustomDepthStencilValue = 0.f;

			SkeletalMeshComponent->MarkRenderStateDirty();

			return;
		}

		if (ScannedMeshComponents.Num() > 0)
		{
			for (auto Comp : ScannedMeshComponents)
			{
				if (Comp == nullptr) { continue; }

				Comp->bRenderCustomDepth = false;

				Comp->CustomDepthStencilValue = 0.f;

				Comp->MarkRenderStateDirty();
			}
		}
	}
}

void UFGRadarTrackingTarget::TickComponent(float DeltaTime, ELevelTick TickType,

                                           FActorComponentTickFunction* ThisTickFunction)

{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsVisibleOnScanner)
	{
		ScannedMeshLifeTimer += DeltaTime;

		if (ScannedMeshLifeTimer >= ScannedMeshLifetime)
		{
			ScannedMeshLifeTimer = 0.f;

			DeactivateVisibleOnScanner();
		}
	}
}
