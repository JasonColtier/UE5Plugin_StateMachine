// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateMachine.h"

#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FStateMachineModule"

void FStateMachineModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UGameplayTagsManager::Get().AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UE5Plugin_StateMachine/Config/Tags"));
}

void FStateMachineModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStateMachineModule, StateMachine)