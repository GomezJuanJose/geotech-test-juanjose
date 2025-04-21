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
	BoardModelData->OnWinGameDelegate.AddRaw(this, &SGameBoardViewController::OnWinGame);
	BoardModelData->OnLoseGameDelegate.AddRaw(this, &SGameBoardViewController::OnLose);

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
	const TArray<TArray<ETileStatus>> BoardData = BoardModelData->CreateLogicalBoard(NewWidth, NewHeight, NumberOfMines);
	BoardGridPanel->ClearChildren();
	VisualBoard.Empty();

	FScopedSlowTask SlowTask(NewWidth, LOCTEXT("DialogLoading", "Building board..."));
	SlowTask.MakeDialog();
	
	for (int32 Row = 0; Row < NewWidth; Row++)
	{
		TArray<TSharedPtr<SButton>>Buttons;
		for (int32 Column = 0; Column < NewHeight; Column++)
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
		VisualBoard.Add(Buttons);
		SlowTask.EnterProgressFrame(1);
	}
}

void SGameBoardViewController::UpdateTileStyle(FTileCoordinate InCoordinate, ETileStatus TileStatus)
{
	FText TileText = FText::FromString("");
	if (TileStatus >= ETileStatus::REVEALED)
	{
		TileText = FText::FromString(FString::FromInt(TileStatus));
	}else if (TileStatus == ETileStatus::MINE)
	{
		TileText = FText::FromString("X");
	}

	VisualBoard[InCoordinate.Row][InCoordinate.Column]->SetContent(
		SNew(STextBlock).Text(TileText).Margin(FMargin(0.0f))
	);
	
	VisualBoard[InCoordinate.Row][InCoordinate.Column]->SetEnabled(false);
}

void SGameBoardViewController::EnableBoard(bool Enable)
{
	for (int32 WidthBoard = 0; WidthBoard < VisualBoard.Num(); WidthBoard++)
	{
		for (int32 HeightBoard = 0; HeightBoard < VisualBoard[0].Num(); HeightBoard++)
		{
			VisualBoard[WidthBoard][HeightBoard]->SetEnabled(Enable);
		}
	}
}

void SGameBoardViewController::RevealMines()
{
	const TArray<FTileCoordinate> MinesCoords = BoardModelData->GetMinesCoordinates();
	for (int32 MineIndex = 0; MineIndex < MinesCoords.Num(); MineIndex++)
	{
		UpdateTileStyle(MinesCoords[MineIndex], ETileStatus::MINE);
	}
}

void SGameBoardViewController::OnWinGame()
{
	EnableBoard(false);
	FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Ok, LOCTEXT("GameState", "You Win!"), LOCTEXT("GameState", "Congratulations!"));
}

void SGameBoardViewController::OnLose()
{
	EnableBoard(false);
	RevealMines();
	FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Ok, LOCTEXT("GameState", "You Lose!"), LOCTEXT("GameState", "Sorry"));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
