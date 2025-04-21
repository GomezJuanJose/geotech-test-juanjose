// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GameBoard/GameBoardViewController.h"

#include "IContentBrowserSingleton.h"
#include "IPropertyTable.h"
#include "SlateOptMacros.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/GameBoard/GameBoardModelData.h"
#include "Resources/MinesweeperEnums.h"
#include "Resources/MinesweeperStructs.h"

#define LOCTEXT_NAMESPACE "MinesweepToolEditorModule"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameBoardViewController::Construct(const FArguments& InArgs)
{
	BoardGridPanel = SNew(SGridPanel);
	BoardModelData = MakeShared<FGameBoardModelData>();
	BoardModelData->OnTileRevealedDelegate.AddRaw(this, &SGameBoardViewController::UpdateTileStyle);
	BoardModelData->OnWinGameDelegate.AddLambda([this]()
	{
		EnableBoard(false);
		FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Ok, LOCTEXT("GameState", "You Win!"), LOCTEXT("GameState", "Congratulations!"));
	});

	BoardModelData->OnLoseGameDelegate.AddLambda([this]()
	{
		EnableBoard(false);
		//TODO: reveal mines
		FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Ok, LOCTEXT("GameState", "You Lose!"), LOCTEXT("GameState", "Sorry"));
	});

	BuildBoard(InArgs._InitialWidth.Get(), InArgs._InitialHeight.Get(), InArgs._InitialMines.Get());
	
	ChildSlot
	[
		BoardGridPanel.ToSharedRef()
	];

}

SGameBoardViewController::~SGameBoardViewController()
{
	BoardModelData = nullptr;
}

void SGameBoardViewController::BuildBoard(int32 NewWidth, int32 NewHeight, int32 NumberOfMines)
{
	TArray<TArray<ETileStatus>> BoardData = BoardModelData->CreateLogicalBoard(NewWidth, NewHeight, NumberOfMines);
	BoardGridPanel->ClearChildren();
	ButtonTiles.Empty();
	
	for (int32 Row = 0; Row < BoardData.Num(); Row++)
	{
		TArray<TSharedPtr<SButton>>Buttons;
		for (int32 Column = 0; Column < BoardData[0].Num(); Column++)
		{
			TSharedPtr<SButton> Button = SNew(SButton).Text(FText::FromString("  "));
			Button->SetOnClicked(FOnClicked::CreateLambda(
				[this, Row, Column]()
				{
					BoardModelData->SelectTile(Row, Column);
					return FReply::Handled();
				}
			));
			BoardGridPanel->AddSlot(Row,Column)
			[
				Button.ToSharedRef()
			];
			Buttons.Add(Button);
		}
		ButtonTiles.Add(Buttons);
	}
}

void SGameBoardViewController::UpdateTileStyle(FTileCoordinate InCoordinate, ETileStatus TileStatus)
{
	FText TileText = FText::FromString("");
	if (TileStatus >= ETileStatus::REVEALED)
	{
		TileText = FText::FromString(FString::FromInt(TileStatus));
	}

	
	ButtonTiles[InCoordinate.Row][InCoordinate.Column]->SetContent(
		SNew(STextBlock).Text(TileText).Margin(FMargin(0.0f))
	);
	
	ButtonTiles[InCoordinate.Row][InCoordinate.Column]->SetEnabled(false);
}

void SGameBoardViewController::EnableBoard(bool Enable)
{
	
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
