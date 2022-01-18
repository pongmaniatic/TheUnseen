// Fill out your copyright notice in the Description page of Project Settings.

#include "FGScannedStencilMesh.h"

#include "GAME/GAME.h"

AFGScannedStencilMesh::AFGScannedStencilMesh()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;
	StaticMesh->bRenderCustomDepth = false;

	StaticMesh->CustomDepthStencilValue = 0.f;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	InitialLifeSpan = 2.f;
	SetActorEnableCollision(false);
}

AFGScannedStencilMesh::AFGScannedStencilMesh(float _InitialLifeSpan)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->bRenderCustomDepth = false;

	StaticMesh->CustomDepthStencilValue = StencilValue;
	InitialLifeSpan = _InitialLifeSpan;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SetActorEnableCollision(false);
}

void AFGScannedStencilMesh::BeginPlay()
{
	Super::BeginPlay();
	Activate();
}

void AFGScannedStencilMesh::LifeSpanExpired()
{
	Destroy();
	MarkPendingKill();
}

void AFGScannedStencilMesh::Tick(float DeltaSeconds) { Super::Tick(DeltaSeconds); }

void AFGScannedStencilMesh::Activate()
{
	StaticMesh->SetRenderCustomDepth(true);
	StaticMesh->CustomDepthStencilValue = StencilValue;
	bIsActivated = true;
	StaticMesh->MarkRenderStateDirty();
}

void AFGScannedStencilMesh::Deactivate()
{
	StaticMesh->bRenderCustomDepth = false;
	StaticMesh->CustomDepthStencilValue = 0;
	bIsActivated = false;
	StaticMesh->MarkRenderStateDirty();
}
