// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


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
};
