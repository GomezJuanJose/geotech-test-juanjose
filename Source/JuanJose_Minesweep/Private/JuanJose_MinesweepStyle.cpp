// Copyright Epic Games, Inc. All Rights Reserved.

#include "JuanJose_MinesweepStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FJuanJose_MinesweepStyle::StyleInstance = nullptr;

void FJuanJose_MinesweepStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FJuanJose_MinesweepStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FJuanJose_MinesweepStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("JuanJose_MinesweepStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush(RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush( Style->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon80x80(80.0f, 80.0f);

TSharedRef< FSlateStyleSet > FJuanJose_MinesweepStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("JuanJose_MinesweepStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("JuanJose_Minesweep")->GetBaseDir() / TEXT("Resources"));

	Style->Set("JuanJose_Minesweep.OpenPluginWindow", new IMAGE_BRUSH(TEXT("Icon40x40"), Icon40x40));

	return Style;
}

void FJuanJose_MinesweepStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FJuanJose_MinesweepStyle::Get()
{
	return *StyleInstance;
}
