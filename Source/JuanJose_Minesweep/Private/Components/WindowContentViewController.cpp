// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WindowContentViewController.h"

#include "SlateOptMacros.h"
#include "Components/GameBoard/GameBoardViewController.h"

#define LOCTEXT_NAMESPACE "MinesweepToolEditorModule"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SWindowContentViewController::Construct(const FArguments& InArgs)
{
	TSharedRef<SVerticalBox> VertialBoxRoot = SNew(SVerticalBox);

	InputHeight = SNew(SSpinBox<int32>).Value(5).Delta(1).MinValue(1).MaxValue(100);
	InputWidth = SNew(SSpinBox<int32>).Value(5).Delta(1).MinValue(1).MaxValue(100);
	InputMines = SNew(SSpinBox<int32>).Value(5).Delta(1).MinValue(1).MaxValue(100);

	VertialBoxRoot->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("WidthSetting", "Width"))
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			InputWidth.ToSharedRef()
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("HeightSetting", "Height"))
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			InputHeight.ToSharedRef()
		]
	];

	VertialBoxRoot->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("MinesNumberSetting", "Number Of Mines"))
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
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
			.OnClicked(this, &SWindowContentViewController::OnClickBuildBoard)
		]
	];
	
	GameBoard = SNew(SGameBoardViewController)
		.InitialWidth(InputWidth->GetValue())
		.InitialHeight(InputHeight->GetValue())
		.InitialMines(InputMines->GetValue());
	
	VertialBoxRoot->AddSlot()
	.AutoHeight()
	[
		GameBoard.ToSharedRef()
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
	
	if (Width * Height < Mines)
	{
		Mines = (Width * Height) - 1;
		bCanBuildBoard = EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("DialogWarn", "You are generating a board without free tiles. The generator will free one. \nDo you want to proceed?"));
	}
		
	if (bCanBuildBoard)
	{
		GameBoard->BuildBoard(Width, Height, Mines);
	}

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
