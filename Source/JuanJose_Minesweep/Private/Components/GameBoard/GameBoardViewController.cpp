// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GameBoard/GameBoardViewController.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameBoardViewController::Construct(const FArguments& InArgs)
{
	BoardGridPanel = SNew(SGridPanel);

	ChildSlot
	[
		BoardGridPanel.ToSharedRef()
	];

}

void SGameBoardViewController::BuildBoard(int32 NewWidth, int32 NewHeight, int32 NumberOfMines)
{
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
