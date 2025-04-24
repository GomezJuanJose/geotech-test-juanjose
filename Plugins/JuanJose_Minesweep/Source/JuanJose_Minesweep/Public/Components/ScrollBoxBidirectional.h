// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class JUANJOSE_MINESWEEP_API SScrollBoxBidirectional : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScrollBoxBidirectional)
		: _Content()
		{
		}

		SLATE_DEFAULT_SLOT( FArguments, Content )
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	void SetContent(TSharedRef< SWidget > InContent);
};
