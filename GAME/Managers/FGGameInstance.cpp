// Fill out your copyright notice in the Description page of Project Settings.

#include "FGGameInstance.h"



UFGGameInstance::UFGGameInstance() {}

void UFGGameInstance::Init()

{
	Super::Init();
}

UFGGameInstance* UFGGameInstance::Get()

{
	static TWeakObjectPtr<UFGGameInstance> Instance;

	if (Instance.IsValid()) { return Instance.Get(); }

	Instance = NewObject<UFGGameInstance>();

	Instance->Init();

	if (Instance.IsValid()) { Instance->AddToRoot(); }

	return Instance.Get();
}
