// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SSpinBox.h"


class SGameBoardViewController;
/**
 * 
 */
class JUANJOSE_MINESWEEP_API SWindowContentViewController : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWindowContentViewController)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	FReply OnClickBuildBoard();
	
private:
	TSharedPtr<SGameBoardViewController> GameBoard;

	TSharedPtr<SSpinBox<int32>> InputHeight;
	TSharedPtr<SSpinBox<int32>> InputWidth;
	TSharedPtr<SSpinBox<int32>> InputMines;
};
