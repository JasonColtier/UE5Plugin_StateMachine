﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SM_MasterState.h"
#include "SM_StateMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeState,ASM_MasterState*,CurrentState);

UCLASS(Blueprintable,BlueprintType)
class STATEMACHINE_API ASM_StateMachine : public ASM_MasterState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASM_StateMachine();
	
	/**
	 * @brief the initial state of the state machine
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="StateMachine")
	TSoftClassPtr<ASM_MasterState> InitialStateClass;

	/**
	* @brief 
	* @param newState Changes the current state of the state machine to the new state. The new state is instantiated
	*/
	UFUNCTION(BlueprintCallable)
	void ChangeState(TSoftClassPtr<ASM_MasterState> newState);


	UFUNCTION(BlueprintCallable)
	void ForceNextState();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief the current state of the state machine
	 */
	UPROPERTY(BlueprintReadOnly,Category="StateMachine")
	TObjectPtr<ASM_MasterState> CurrentState;

	/**
	 * @brief called when a state is changed
	 */
	UPROPERTY(BlueprintAssignable,Category="StateMachine")
	FOnChangeState OnChangeStateDelegate;

	/**
	 * The states that have allready been instantiated
	 */
	UPROPERTY(BlueprintReadOnly,Category="StateMachine")
	TArray<TObjectPtr<ASM_MasterState>> LoadedStates;

	/**
	 * 
	 * @param stateClass the state class wanted
	 * @return the loaded AState* if found, nullptr if not found
	 */
	TObjectPtr<ASM_MasterState> FindLoadedState(TSoftClassPtr<ASM_MasterState> stateClass);

	/**
 * @brief Checks the transition of the current state to see if it can transition to another state
 */
	UFUNCTION()
	void CheckTransitions();

	/**
	 * Checks if the current state has no transition to another state = the end of the chain
	 */
	UFUNCTION(BlueprintCallable)
	bool CurrentStateIsLeafState();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
