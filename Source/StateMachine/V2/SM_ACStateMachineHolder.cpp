// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ACStateMachineHolder.h"

#include "SM_MasterState.h"


// Sets default values for this component's properties
USM_ACStateMachineHolder::USM_ACStateMachineHolder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USM_ACStateMachineHolder::BeginPlay()
{
	Super::BeginPlay();

	if (initialStateClass != nullptr)
	{
		ASM_MasterState* LoadedState = GetWorld()->SpawnActor<ASM_MasterState>(initialStateClass.LoadSynchronous());
		LoadedState->OnEnterState();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("no initial state"));
	}
	
}


// Called every frame
void USM_ACStateMachineHolder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

