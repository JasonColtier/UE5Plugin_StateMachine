// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "AC_StateMachine.h"
// #include "UObject/NoExportTypes.h"
// #include "State.generated.h"
//
// class UAC_StateMachine;
// DECLARE_DYNAMIC_DELEGATE_TwoParams(FTransitionDelegate, TSoftClassPtr<AState>&, nextState, bool&, canTransition);
//
//
// /**
//  * 
//  */
// UCLASS()
// class STATEMACHINE_API AState : public AActor
// {
// 	GENERATED_BODY()
//
//
//
// public:
//
// 	UPROPERTY(BlueprintReadOnly)
// 	int StartedTime {0};
//
// 	UPROPERTY(BlueprintReadOnly)
// 	int FinishedTime {0};
//
// 	UPROPERTY(BlueprintReadOnly)
// 	TEnumAsByte<EStateProgress> currentProgressState = notStarted;
// 	
// 	/**
// 	 * @brief La référence à la state machine
// 	 */
// 	UPROPERTY(BlueprintReadOnly)
// 	UAC_StateMachine* StateMachineRef;
//
// 	/**
// 	 * @brief the default transition of this state, can be configured in BP
// 	 */
// 	UPROPERTY()
// 	FTransitionDelegate defaultTransitionDelegate;
//
// 	/**
// 	 * @brief the transitions of this state to other states
// 	 */
// 	TArray<FTransitionDelegate> transitionsArray;
//
// 	/**
// 	 * @brief Adds a transition to the transition array
// 	 * @param function 
// 	 */
// 	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "function"))
// 	void AddTransition(const FTransitionDelegate& function);
//
// 	/**
// 	* @brief When this state is initialised
// 	*/
// 	UFUNCTION(BlueprintImplementableEvent)
// 	void OnEnterState();
//
// 	/**
// 	 * @brief When this state is exited. The state will be destroyed at the end of the tick
// 	 */
// 	UFUNCTION(BlueprintImplementableEvent)
// 	void OnExitState();
//
// 	/**
// 	 * @brief the default transition of this state
// 	 * @param nextState 
// 	 * @param canTransition 
// 	 */
// 	UFUNCTION(BlueprintImplementableEvent)
// 	void DefaultTransition(TSoftClassPtr<AState>& nextState, bool& canTransition);
//
// 	/**
// 	 * @brief 
// 	 * @param transitions Gets the next states from the current state
// 	 */
// 	UFUNCTION(BlueprintCallable)
// 	void GetTransitions(TArray<TSoftClassPtr<AState>>& transitions);
//
// 	/**
// 	 * If there is no transitions from this state, then it is final
// 	 */
// 	UFUNCTION(BlueprintCallable,BlueprintPure)
// 	bool IsFinalState();
// 	
// protected:
// 	virtual void BeginPlay() override;
// };
