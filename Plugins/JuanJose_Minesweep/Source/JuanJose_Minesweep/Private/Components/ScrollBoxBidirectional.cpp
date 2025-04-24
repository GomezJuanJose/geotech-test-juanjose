// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ScrollBoxBidirectional.h"

#include "SlateOptMacros.h"
#include "Widgets/SCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SScrollBoxBidirectional::Construct(const FArguments& InArgs)
{
	
	SetContent(InArgs._Content.Widget);
}

void SScrollBoxBidirectional::SetContent(TSharedRef<SWidget> InContent)
{
	TSharedPtr<SScrollBar> VerticalScrollBar = SNew(SScrollBar)
				.Orientation(Orient_Vertical)
				.AlwaysShowScrollbar(true)
				.Thickness(10);

	TSharedPtr<SScrollBar> HorizontalScrollBar = SNew(SScrollBar)
				.Orientation(Orient_Horizontal)
				.AlwaysShowScrollbar(true)
				.Thickness(10);

	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(SScrollBox)
				.Orientation(Orient_Vertical)
				.ExternalScrollbar(VerticalScrollBar)
				+ SScrollBox::Slot()
				[
					SNew(SScrollBox)
					.Orientation(Orient_Horizontal)
					.ExternalScrollbar(HorizontalScrollBar)
					+ SScrollBox::Slot()
					[
							InContent
					]
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				VerticalScrollBar.ToSharedRef()
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			HorizontalScrollBar.ToSharedRef()
		]
	]; 
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
