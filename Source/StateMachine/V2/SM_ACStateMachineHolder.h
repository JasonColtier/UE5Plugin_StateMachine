// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SM_ACStateMachineHolder.generated.h"


class ASM_MasterState;

/**
 * this class is the initializer of the state machine
 * if simply initializes the first State and then the state machine runs itself
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STATEMACHINE_API USM_ACStateMachineHolder : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USM_ACStateMachineHolder();

	/**
	 * @brief the initial state of the state machine
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftClassPtr<ASM_MasterState> initialStateClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
