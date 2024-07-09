// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_StateMachine.h"

#include "State.h"

DEFINE_LOG_CATEGORY(StateMachine);

// Sets default values for this component's properties
UAC_StateMachine::UAC_StateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAC_StateMachine::BeginPlay()
{
	Super::BeginPlay();


	if (initialStateClass != nullptr)
	{
		if(loadAllStatesOnBeginPlay)
		{
			LoadAllStateRecursively(initialStateClass);
		}
		
		ChangeState(initialStateClass);
	}
	else
	{
		UE_LOG(StateMachine, Error, TEXT("no initial state"));
	}
}


// Called every frame
void UAC_StateMachine::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!currentState)
		return;


	CheckTransitions();
}

void UAC_StateMachine::ChangeState(TSoftClassPtr<AState>& newState)
{
	if (newState.IsNull())
	{
		UE_LOG(StateMachine, Error, TEXT("invalid state passed as parameter"));
		return;
	}

	//if a state was allready running
	if (currentState != nullptr)
	{
		currentState->FinishedTime = GetWorld()->GetTimeSeconds();
		currentState->OnExitState();
		currentState->currentProgressState = finished;
	}

	//we try to get the loaded state if it exists
	const auto loadedState = FindLoadedState(newState);
	
	if(IsValid(loadedState))
	{
		currentState = loadedState;
	}else
	{
		currentState = GetWorld()->SpawnActor<AState>(newState.LoadSynchronous(),GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		loadedStates.AddUnique(currentState);
	}
	
	currentState->StateMachineRef = this;
	currentState->StartedTime = GetWorld()->GetTimeSeconds();
	currentState->currentProgressState = inProgress;
	currentState->OnEnterState();
	OnChangeStateDelegate.Broadcast(currentState);
}

void UAC_StateMachine::ForceChangeState(TSoftClassPtr<AState> newState)
{
	if (newState.IsNull())
	{
		UE_LOG(StateMachine, Error, TEXT("invalid state passed as parameter when force loading"));
		return;
	}
	
	UE_LOG(StateMachine, Log, TEXT("forcing state change"));
	ChangeState(newState);
}

void UAC_StateMachine::CheckTransitions()
{
	if(!IsValid(currentState))
	{
		UE_LOG(StateMachine, Error, TEXT("invalid current state !"));
		return;
	}

	if(currentState->transitionsArray.IsEmpty())
	{
		UE_LOG(StateMachine, Error, TEXT("no transition found !"));
		return;
	}
	
	for (const auto& transition : currentState->transitionsArray)
	{
		bool canTransition;
		TSoftClassPtr<AState> nextState;
		transition.Execute(nextState, canTransition);
		if (canTransition)
		{
			ChangeState(nextState);
			return;
		}
	}
	// UE_LOG(StateMachine, Log, TEXT("cannot transition from current state"));
}

TObjectPtr<AState> UAC_StateMachine::FindLoadedState(TSoftClassPtr<AState> stateClass)
{
	if(stateClass.IsNull())
		return nullptr;

	UClass* loadedState = stateClass.LoadSynchronous();
	
	for (auto state : loadedStates)
	{
		if(state->IsA(loadedState))
		{
			return state;
		}
	}
	return nullptr;
}

void UAC_StateMachine::LoadAllStateRecursively(TSoftClassPtr<AState> state)
{
	//if the state is null or allready laoded
	if(state.IsNull() || FindLoadedState(state))
		return;

	auto loadedState = GetWorld()->SpawnActor<AState>(state.LoadSynchronous(),GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	loadedStates.AddUnique(loadedState);
	loadedState->StateMachineRef = this;

	TArray<TSoftClassPtr<AState>> transitions;
	loadedState->GetTransitions(transitions);

	if(transitions.IsEmpty())
		return;
	
	for (auto transition : transitions)
	{
		LoadAllStateRecursively(transition);
	}
}
