// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "JuanJose_MinesweepStyle.h"

class FJuanJose_MinesweepCommands : public TCommands<FJuanJose_MinesweepCommands>
{
public:

	FJuanJose_MinesweepCommands()
		: TCommands<FJuanJose_MinesweepCommands>(TEXT("JuanJose_Minesweep"), NSLOCTEXT("Contexts", "JuanJose_Minesweep", "JuanJose_Minesweep Plugin"), NAME_None, FJuanJose_MinesweepStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};