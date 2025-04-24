// Copyright Epic Games, Inc. All Rights Reserved.

#include "JuanJose_MinesweepCommands.h"

#define LOCTEXT_NAMESPACE "FJuanJose_MinesweepModule"

void FJuanJose_MinesweepCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "JuanJose_Minesweep", "Bring up JuanJose_Minesweep window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
