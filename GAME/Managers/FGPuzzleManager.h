// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FGPuzzleManager.generated.h"

UCLASS()
class GAME_API AFGPuzzleManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFGPuzzleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetPuzzleFinished();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Puzzle)
	TArray<AActor*> puzzlePieces;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Puzzle)
	TArray<AActor*> completedPuzzleResult;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Puzzle)
	int numberOfPieces = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Puzzle)
	int numberOfCompletedPieces = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Puzzle)
	bool puzzleFinished = false;

	virtual void Tick(float DeltaTime) override;
};
