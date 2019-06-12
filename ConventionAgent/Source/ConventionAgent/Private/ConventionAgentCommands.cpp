// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ConventionAgentCommands.h"

#define LOCTEXT_NAMESPACE "FConventionAgentModule"

void FConventionAgentCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ConventionAgent", "Bring up ConventionAgent window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
