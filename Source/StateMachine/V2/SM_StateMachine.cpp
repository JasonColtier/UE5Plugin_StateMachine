// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_StateMachine.h"


// Sets default values
ASM_StateMachine::ASM_StateMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASM_StateMachine::ChangeState(TSoftClassPtr<ASM_MasterState> newState)
{
	if (newState.IsNull())
	{
		UE_LOG(SM_StateMachine, Error, TEXT("invalid state passed as parameter"));
		return;
	}

	//if a state was allready running
	if (CurrentState != nullptr)
	{
		CurrentState->OnExitState();
	}

	//we try to get the loaded state if it exists
	const auto loadedState = FindLoadedState(newState);
	
	if(IsValid(loadedState))
	{
		CurrentState = loadedState;
	}else
	{
		CurrentState = GetWorld()->SpawnActor<ASM_MasterState>(newState.LoadSynchronous());
		LoadedStates.AddUnique(CurrentState);
	}
	
	CurrentState->ParentStateMachineRef = this;
	CurrentState->OnEnterState();
	OnChangeStateDelegate.Broadcast(CurrentState);
}

// Called when the game starts or when spawned
void ASM_StateMachine::BeginPlay()
{
	Super::BeginPlay();

	if(InitialStateClass != nullptr)
		ChangeState(InitialStateClass);
}

TObjectPtr<ASM_MasterState> ASM_StateMachine::FindLoadedState(TSoftClassPtr<ASM_MasterState> stateClass)
{
	
	if(stateClass.IsNull())
		return nullptr;

	UClass* loadedState = stateClass.LoadSynchronous();
	
	for (auto state : LoadedStates)
	{
		if(state->IsA(loadedState))
		{
			return state;
		}
	}
	return nullptr;
}

void ASM_StateMachine::CheckTransitions()
{
	if(!IsValid(CurrentState))
	{
		UE_LOG(SM_StateMachine, Error, TEXT("invalid current state !"));
		return;
	}

	if(CurrentState->TransitionsArray.IsEmpty())
	{
		UE_LOG(SM_StateMachine, Error, TEXT("no transition found !"));
		return;
	}
	
	for (const auto& transition : CurrentState->TransitionsArray)
	{
		bool canTransition;
		TSoftClassPtr<ASM_MasterState> nextState;
		transition.Execute(nextState, canTransition);
		if (canTransition)
		{
			ChangeState(nextState);
			return;
		}
	}
	// UE_LOG(StateMachine, Log, TEXT("cannot transition from current state"));
}

// Called every frame
void ASM_StateMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckTransitions();
}

