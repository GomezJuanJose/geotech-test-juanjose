// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class JUANJOSE_MINESWEEP_API SGameBoardViewController : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameBoardViewController)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void BuildBoard(int32 NewWidth, int32 NewHeight, int32 NumberOfMines);
private:
	TSharedPtr<SGridPanel> BoardGridPanel;
};
