// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_MasterState.h"


DEFINE_LOG_CATEGORY(SM_StateMachine)

// Sets default values
ASM_MasterState::ASM_MasterState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASM_MasterState::OnEnterState()
{
	CurrentProgress = InProgress;
	StartedTime = GetWorld()->GetTimeSeconds();

	OnStateEntered();
}

void ASM_MasterState::OnExitState()
{
	CurrentProgress = EStateProgressStatus::Finished;
	FinishedTime = GetWorld()->GetTimeSeconds();

	OnExitingState();
}

void ASM_MasterState::GetTransitions(TArray<TSoftClassPtr<ASM_MasterState>>& transitions)
{
	for (const auto& transition : TransitionsArray)
	{
		if (transition.IsBound())
		{
			TSoftClassPtr<ASM_MasterState> nextState;
			bool tmp;
			transition.Execute(nextState, tmp);
			transitions.Add(nextState);
		}
		else
		{
			UE_LOG(SM_StateMachine, Error, TEXT("transition not bound"));
		}
	}
}

// Called when the game starts or when spawned
void ASM_MasterState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(SM_StateMachine, Log, TEXT("state created %s"), *GetName());
}

// Called every frame
void ASM_MasterState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

