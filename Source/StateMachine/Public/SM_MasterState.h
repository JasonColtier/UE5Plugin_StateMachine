// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SM_MasterState.generated.h"

class ASM_StateMachine;
DECLARE_LOG_CATEGORY_EXTERN(SM_StateMachine, Log, All);

UENUM(BlueprintType)
enum EStateProgressStatus
{
	NotStarted,
	InProgress,
	Finished
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FTransitionDelegate, TSoftClassPtr<ASM_MasterState>&, nextState, bool&, canTransition);


/**
 * this class 
 */
UCLASS(Blueprintable,BlueprintType)
class STATEMACHINE_API ASM_MasterState : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASM_MasterState();

	UPROPERTY(EditAnywhere,Category="StateMachine",BlueprintReadOnly)
	FGameplayTag StateID;

	/**
	 * Time at we entered this state
	 */
	UPROPERTY(BlueprintReadOnly,Category="StateMachine")
	int StartedTime{0};

	/**
	 * time we exited this state
	 */
	UPROPERTY(BlueprintReadOnly,Category="StateMachine")
	int FinishedTime{0};

	UPROPERTY(BlueprintReadOnly,Category="StateMachine")
	TEnumAsByte<EStateProgressStatus> CurrentProgress = EStateProgressStatus::NotStarted;

	UPROPERTY(BlueprintReadOnly,Category="StateMachine")
	TObjectPtr<ASM_StateMachine> ParentStateMachineRef;

	/**
	 * @brief the transitions of this state to other states
	 */
	TArray<FTransitionDelegate> TransitionsArray;

	/**
	 * @brief the default transition of this state, can be configured in BP
	 */
	UPROPERTY()
	FTransitionDelegate DefaultTransitionDelegate;

	/**
	* @brief When this state is activated and becomes the current state of the state machine
	*/
	UFUNCTION()
	void OnEnterState();

	/**
	 * BP implementation entrer state
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStateEntered();

	/**
	* @brief When this state is exited
	*/
	UFUNCTION()
	void OnExitState();

	/**
	 * BP implementation exit state
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnExitingState();
	
	/**
	 * @brief the default transition of this state
	 * @param nextState 
	 * @param canTransition 
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void GetDefaultTransition(TSoftClassPtr<ASM_MasterState>& nextState, bool& canTransition);

	/**
	 * @brief 
	 * @param transitions Gets the next states from the current state
	 */
	UFUNCTION(BlueprintCallable)
	void GetTransitions(TArray<TSoftClassPtr<ASM_MasterState>>& transitions);
	
	/**
	 * mark this state as finished
	 */
	UFUNCTION(BlueprintCallable)
	void SetStateFinished();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief draws debug states and transition on screen
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="StateMachine")
	bool showDebug = false;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
