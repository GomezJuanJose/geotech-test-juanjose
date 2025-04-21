// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GameBoard/GameBoardViewController.h"

#include "SlateOptMacros.h"
#include "Components/GameBoard/GameBoardModelData.h"
#include "Resources/MinesweeperEnums.h"
#include "Resources/MinesweeperStructs.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameBoardViewController::Construct(const FArguments& InArgs)
{
	BoardGridPanel = SNew(SGridPanel);
	BoardModelData = MakeShared<FGameBoardModelData>();
	BoardModelData->OnTileRevealedDelegate.AddUObject(this, &SGameBoardViewController::UpdateTileStyle);
	BoardModelData->OnWinGameDelegate.AddLambda([this]()
	{
		EnableBoard(false);
		//TODO: Dialog
	});

	BoardModelData->OnLoseGameDelegate.AddLambda([this]()
	{
		EnableBoard(false);
		//TODO: reveal mines
		//TODO: Dialog
	});

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
	
	for (int32 Row = 0; Row < BoardData.Num(); Row++)
	{
		for (int32 Column = 0; Column < BoardData[0].Num(); Column++)
		{
			TSharedPtr<SButton> Button = SNew(SButton).Text(FText::FromString("  "));
			Button->SetOnClicked(FOnClicked::CreateLambda(
				[this, Row, Column]()
				{
					BoardModelData->SelectTile(Row, Column);
				}
			));
			BoardGridPanel->AddSlot(Row,Column)
			[
				Button.ToSharedRef()
			];
		}
	}
}

void SGameBoardViewController::UpdateTileStyle(FTileCoordinate InCoordinate, ETileStatus TileStatus)
{
	FText TileText = FText::FromString("");
	if (TileStatus >= ETileStatus::REVEALED)
	{
		TileText = FText::FromString(FString::FromInt(TileStatus));
	}
}

void SGameBoardViewController::EnableBoard(bool Enable)
{
	
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
