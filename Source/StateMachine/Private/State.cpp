// Fill out your copyright notice in the Description page of Project Settings.


#include "State.h"

#include "AC_StateMachine.h"


void AState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(StateMachine, Log, TEXT("state created %s"), *GetName());
	defaultTransitionDelegate.BindUFunction(this, "DefaultTransition");
	transitionsArray.AddUnique(defaultTransitionDelegate);
}

void AState::AddTransition(const FTransitionDelegate& function)
{
	transitionsArray.AddUnique(function);
	UE_LOG(StateMachine, Log, TEXT("added transition %s"), *function.GetFunctionName().ToString());
}

void AState::GetTransitions(TArray<TSoftClassPtr<AState>>& transitions)
{
	for (const auto& transition : transitionsArray)
	{
		if (transition.IsBound())
		{
			TSoftClassPtr<AState> nextState;
			bool tmp;
			transition.Execute(nextState, tmp);
			transitions.Add(nextState);
		}
		else
		{
			UE_LOG(StateMachine, Error, TEXT("transition not bound"));
		}
	}
}

bool AState::IsFinalState()
{
	return transitionsArray.IsEmpty();
}

