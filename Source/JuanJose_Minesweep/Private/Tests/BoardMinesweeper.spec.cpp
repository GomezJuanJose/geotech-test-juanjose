// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/GameBoard/GameBoardModelData.h"
#include "Misc/AutomationTest.h"


BEGIN_DEFINE_SPEC(
	FGameBoardModelDataSpec, "JuanJose_MineSweeper.GameBoard",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
		
		TSharedPtr<FGameBoardModelData> GameBoardModelData = GameBoardModelData = MakeShared<FGameBoardModelData>();
		const TArray<TArray<FTileData>>& LogicalBoard = GameBoardModelData->GetLogicalBoard();;

		// Test helpers.
		bool AreBoardsEquals(const TArray<TArray<FTileData>>& BoardA, const TArray<TArray<FTileData>>& BoardB);
END_DEFINE_SPEC(FGameBoardModelDataSpec)

void FGameBoardModelDataSpec::Define()
{

	
	Describe("When creating an 8x8 board with 10 mines", [this](){
		BeforeEach([this]()
		{
			GameBoardModelData->CreateLogicalBoard(8, 8, 10);
		});
		
		It("Should be at the size specified", [this]()
		{
			TestTrue(TEXT("The board has 64 tiles."), LogicalBoard.Num() * LogicalBoard[0].Num() == 64);
		});

		It("Should be filled with ETileStatus::NONE values", [this]()
		{
			for (int32 Row = 0; Row < LogicalBoard.Num(); ++Row)
			{
				for (int32 Column = 0; Column < LogicalBoard[Row].Num(); ++Column)
				{
					if (!GameBoardModelData->CheckTileStatus({Row,Column}, ETileStatus::NONE))
					{
						TestTrue(TEXT("It is not filled with NONE values."), false);
					}
				}
			}

			TestTrue(TEXT("It is filled with NONE values."), true);
		});
	});
	
	

	Describe("When playing a board of 4x4 with 4 mines", [this]()
	{
		BeforeEach([this]()
		{
			GameBoardModelData->CreateLogicalBoard(4, 4, 4, 1);
		});
		
		It("Should generate the specified number of bombs", [this]()
		{
			GameBoardModelData->SelectTile({0, 0});
			TestTrue(TEXT("It has all the specified bombs"), GameBoardModelData->GetMinesCoordinates().Num() == 4);
		});
		It("Should count the number of bombs correctly", [this]()
		{
			GameBoardModelData->SelectTile({0, 0});
			GameBoardModelData->SelectTile({0, 1});
			GameBoardModelData->SelectTile({0, 2});
			GameBoardModelData->SelectTile({0, 3});
			GameBoardModelData->SelectTile({3, 3});

			TArray<TArray<FTileData>> ExpectedBoard =
			{
			{{2, ETileStatus::REVEALED}, {3, ETileStatus::REVEALED}, {3, ETileStatus::REVEALED}, {2, ETileStatus::REVEALED}},
			{{-1, ETileStatus::MINE}, {-1, ETileStatus::MINE}, {-1, ETileStatus::MINE}, {-1, ETileStatus::MINE}},
			{{2, ETileStatus::REVEALED}, {3, ETileStatus::REVEALED}, {3, ETileStatus::REVEALED}, {2, ETileStatus::REVEALED}},
			{{0, ETileStatus::REVEALED}, {0, ETileStatus::REVEALED}, {0, ETileStatus::REVEALED}, {0, ETileStatus::REVEALED}}
			};
			TestTrue(TEXT("Produces the expected result"), AreBoardsEquals(ExpectedBoard, LogicalBoard));
		});

		It("Should should stop revealing tiles when found one with a surrounding bomb", [this]()
		{
			GameBoardModelData->SelectTile({0, 0});
			GameBoardModelData->SelectTile({0, 1});
			GameBoardModelData->SelectTile({0, 2});
			GameBoardModelData->SelectTile({0, 3});
			TArray<TArray<FTileData>> ExpectedBoard =
			{
			{{2, ETileStatus::REVEALED}, {3, ETileStatus::REVEALED}, {3, ETileStatus::REVEALED}, {2, ETileStatus::REVEALED}},
			{{-1, ETileStatus::MINE}, {-1, ETileStatus::MINE}, {-1, ETileStatus::MINE}, {-1, ETileStatus::MINE}},
			{{-1, ETileStatus::NONE}, {-1, ETileStatus::NONE}, {-1, ETileStatus::NONE}, {-1, ETileStatus::NONE}},
			{{-1, ETileStatus::NONE}, {-1, ETileStatus::NONE}, {-1, ETileStatus::NONE}, {-1, ETileStatus::NONE}}
			};
			TestTrue(TEXT("Produces the expected result"), AreBoardsEquals(ExpectedBoard, LogicalBoard));
		});
	});

	Describe("When playing a board of 4x4 with 40 mines", [this]()
	{
		BeforeEach([this]()
		{
			GameBoardModelData->CreateLogicalBoard(4, 4, 4, 1);
		});
		It("Should be two free tiles", [this]()
		{
			int32 NumberOfFreeTiles = 0;
			for (int32 Row = 0; Row < LogicalBoard.Num(); Row++)
			{
				for (int32 Column = 0; Column < LogicalBoard[Row].Num(); Column++)
				{
					if (GameBoardModelData->CheckTileStatus({Row,Column}, ETileStatus::NONE))
					{
						NumberOfFreeTiles++;
					}
				}
			}
						
			TestTrue(TEXT("It does not have at least two free tiles"), NumberOfFreeTiles >= 2);
		});
	});
	
	Describe("When starts new games over and over", [this]()
	{
		It("Should not crash due to a stack overflow error or an index out of bounds error", [this]()
		{
			for (int32 i = 0; i < 20; ++i)
			{
				GameBoardModelData->CreateLogicalBoard(8, 8, 10);
				GameBoardModelData->SelectTile({0, 0});
			}
			TestTrue(TEXT("Not crash"), true);
		});
	});
}

bool FGameBoardModelDataSpec::AreBoardsEquals(const TArray<TArray<FTileData>>& BoardA,
	const TArray<TArray<FTileData>>& BoardB)
{
	if (BoardA.Num() != BoardB.Num() || BoardA[0].Num() != BoardB[0].Num())
	{
		return false;
	}

	for (int32 Row = 0; Row < BoardA.Num(); ++Row)
	{
		for (int32 Column = 0; Column < BoardA[Row].Num(); ++Column)
		{
			if (BoardA[Row][Column].Status != BoardB[Row][Column].Status || BoardA[Row][Column].SurroundingMines != BoardB[Row][Column].SurroundingMines)
			{
				return false;
			}
		}
	}
	
	return true;
}

