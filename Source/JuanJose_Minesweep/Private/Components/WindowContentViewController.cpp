// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WindowContentViewController.h"

#include "SlateOptMacros.h"
#include "Components/GameBoard/GameBoardViewController.h"

#include "Components/ScrollBoxBidirectional.h"

#define LOCTEXT_NAMESPACE "MinesweepToolEditorModule"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SWindowContentViewController::Construct(const FArguments& InArgs)
{
	TSharedRef<SVerticalBox> VertialBoxRoot = SNew(SVerticalBox);

	InputHeight = SNew(SSpinBox<int32>).Value(8).Delta(1).MinValue(3).MaxValue(100);
	InputWidth = SNew(SSpinBox<int32>).Value(8).Delta(1).MinValue(3).MaxValue(100);
	InputMines = SNew(SSpinBox<int32>).Value(10).Delta(1).MinValue(1).MaxValue(100);

	VertialBoxRoot->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("WidthSetting", "Width: "))
		]
		+SHorizontalBox::Slot()
		.MaxWidth(200)
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Fill)
		[
			InputWidth.ToSharedRef()
		]
		
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("HeightSetting", "Height: "))
		]
		+SHorizontalBox::Slot()
		.MaxWidth(200)
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Fill)
		[
			InputHeight.ToSharedRef()
		]
	];

	VertialBoxRoot->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("MinesNumberSetting", "Number Of Mines: "))
		]
		+SHorizontalBox::Slot()
		.MaxWidth(200)
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Fill)
		[
			InputMines.ToSharedRef()
		]
	];

	VertialBoxRoot->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SButton)
			.Text(LOCTEXT("GenerateBoardAction", "Generate New Board"))
			.HAlign(HAlign_Center)
			.OnClicked(this, &SWindowContentViewController::OnClickBuildBoard)
		]
	];
	
	GameBoard = SNew(SGameBoardViewController)
		.InitialWidth(InputWidth->GetValue())
		.InitialHeight(InputHeight->GetValue())
		.InitialMines(InputMines->GetValue());
	
	VertialBoxRoot->AddSlot()
	[
		SNew(SScrollBoxBidirectional)
		[
			GameBoard.ToSharedRef()
		]
	];
	
	ChildSlot
	[
		VertialBoxRoot
	];
}

FReply SWindowContentViewController::OnClickBuildBoard()
{
	bool bCanBuildBoard = true;
	int32 Width = InputWidth->GetValue();
	int32 Height = InputHeight->GetValue();
	int32 Mines = InputMines->GetValue();
	
	if (Mines >= Width * Height)
	{
		bCanBuildBoard = EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("DialogWarn", "You are generating a board without free tiles. The generator will free two. \nDo you want to proceed?"));
	}
		
	if (bCanBuildBoard)
	{
		GameBoard->CreateBoard(Width, Height, Mines);
	}

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
