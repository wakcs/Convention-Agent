// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ConventionAgentStyle.h"

class FConventionAgentCommands : public TCommands<FConventionAgentCommands>
{
public:

	FConventionAgentCommands()
		: TCommands<FConventionAgentCommands>(TEXT("ConventionAgent"), NSLOCTEXT("Contexts", "ConventionAgent", "ConventionAgent Plugin"), NAME_None, FConventionAgentStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};