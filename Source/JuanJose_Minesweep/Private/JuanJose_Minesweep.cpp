// Copyright Epic Games, Inc. All Rights Reserved.

#include "JuanJose_Minesweep.h"
#include "JuanJose_MinesweepStyle.h"
#include "JuanJose_MinesweepCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Components/WindowContentViewController.h"

static const FName JuanJose_MinesweepTabName("JuanJose_Minesweep");

#define LOCTEXT_NAMESPACE "FJuanJose_MinesweepModule"

void FJuanJose_MinesweepModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FJuanJose_MinesweepStyle::Initialize();
	FJuanJose_MinesweepStyle::ReloadTextures();

	FJuanJose_MinesweepCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FJuanJose_MinesweepCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FJuanJose_MinesweepModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FJuanJose_MinesweepModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(JuanJose_MinesweepTabName, FOnSpawnTab::CreateRaw(this, &FJuanJose_MinesweepModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FJuanJose_MinesweepTabTitle", "JuanJose_Minesweep"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FJuanJose_MinesweepModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FJuanJose_MinesweepStyle::Shutdown();

	FJuanJose_MinesweepCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(JuanJose_MinesweepTabName);
}

TSharedRef<SDockTab> FJuanJose_MinesweepModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FJuanJose_MinesweepModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("JuanJose_Minesweep.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SWindowContentViewController)
		];
}

void FJuanJose_MinesweepModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(JuanJose_MinesweepTabName);
}

void FJuanJose_MinesweepModule::RegisterMenus()
{
	
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			FSlateIcon Icon = FSlateIcon(FJuanJose_MinesweepStyle::GetStyleSetName(), "JuanJose_Minesweep.OpenPluginWindow");
			Section.AddMenuEntryWithCommandList(FJuanJose_MinesweepCommands::Get().OpenPluginWindow, PluginCommands, TAttribute<FText>(), TAttribute<FText>(), Icon);
			
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FJuanJose_MinesweepCommands::Get().OpenPluginWindow));
				FSlateIcon Icon = FSlateIcon(FJuanJose_MinesweepStyle::GetStyleSetName(), "JuanJose_Minesweep.OpenPluginWindow");
				Section.AddMenuEntryWithCommandList(FJuanJose_MinesweepCommands::Get().OpenPluginWindow, PluginCommands, TAttribute<FText>(), TAttribute<FText>(), Icon);
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJuanJose_MinesweepModule, JuanJose_Minesweep)