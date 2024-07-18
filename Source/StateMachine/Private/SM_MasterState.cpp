// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_MasterState.h"

#include "Net/UnrealNetwork.h"


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
	if(CurrentProgress == InProgress)
		SetStateFinished();

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

void ASM_MasterState::SetStateFinished()
{
	CurrentProgress = Finished;
	FinishedTime = GetWorld()->GetTimeSeconds();
}

// Called when the game starts or when spawned
void ASM_MasterState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(SM_StateMachine, Log, TEXT("state created %s"), *GetName());

	DefaultTransitionDelegate.BindUFunction(this, "GetDefaultTransition");

	TSoftClassPtr<ASM_MasterState> NextState;
	bool CanTrans;
	DefaultTransitionDelegate.Execute(NextState,CanTrans);
	if(!NextState.IsNull())
		TransitionsArray.AddUnique(DefaultTransitionDelegate);
}

void ASM_MasterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASM_MasterState,StateID);
}

// Called every frame
void ASM_MasterState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


