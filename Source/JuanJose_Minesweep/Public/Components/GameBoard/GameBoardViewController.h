// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FTileCoordinate;
enum ETileStatus : int32;
class FGameBoardModelData;
/**
 * 
 */
class JUANJOSE_MINESWEEP_API SGameBoardViewController : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameBoardViewController)
		: _InitialWidth()
		, _InitialHeight()
		, _InitialMines()
		{
		}

		SLATE_ATTRIBUTE(int32, InitialWidth)
		SLATE_ATTRIBUTE(int32, InitialHeight)
		SLATE_ATTRIBUTE(int32, InitialMines)
		
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual ~SGameBoardViewController() override;

	void CreateVisualBoard(int32 NewWidth, int32 NewHeight, int32 NumberOfMines);

private:
	void UpdateTileStyle(FTileCoordinate InCoordinate, ETileStatus TileStatus);
	void EnableBoard(bool Enable);
	void RevealMines();

	void OnWinGame();
	void OnLose();
	
private:
	TSharedPtr<SGridPanel> BoardGridPanel;
	TArray<TArray<TSharedPtr<SButton>>> VisualBoard;
	
	TSharedPtr<FGameBoardModelData> BoardModelData;
};
