#include "Components/GameBoard/GameBoardModelData.h"
#include "Resources/MinesweeperEnums.h"
#include "Resources/MinesweeperStructs.h"

FGameBoardModelData::FGameBoardModelData()
{
	WidthBoard = 0;
	HeightBoard = 0;
	Mines = 0;
	bIsFirstOpen = false;
}

void FGameBoardModelData::CreateLogicalBoard(int32 InWidth, int32 InHeight, int32 InNumberOfMines)
{
	LogicalBoard.Empty();
	MinesCoords.Empty();

	WidthBoard = InWidth;
	HeightBoard = InHeight;
	Mines = InNumberOfMines;
	RevealedTileCount = 0;
	bIsFirstOpen = false;
	
	for (int32 Row = 0; Row < InWidth; Row++)
	{
		TArray<FTileData> TileRow;
		TileRow.Init({-1, ETileStatus::NONE}, InHeight);
		LogicalBoard.Add(TileRow);
	}
}

void FGameBoardModelData::SelectTile(int32 InRow, int32 InColumn)
{
	
	
	if (bIsFirstOpen == false)
	{
		bIsFirstOpen = true;
		SpawnMines(Mines, InRow, InColumn);
	}
				
	if (LogicalBoard[InRow][InColumn].Status == ETileStatus::MINE) // Never hits the condition on the first click because the mines are spawned after the click
	{
		OnLoseGameDelegate.Broadcast();
		return;
	}
		
	RevealTile(InRow, InColumn);

	if (RevealedTileCount == (WidthBoard * HeightBoard) - Mines)
	{
		OnWinGameDelegate.Broadcast();
	}
}

const TArray<FTileCoordinate>& FGameBoardModelData::GetMinesCoordinates() const
{
	return MinesCoords;
}

int32 FGameBoardModelData::RevealTile(int32 Row, int32 Column)
{
	if (!LogicalBoard.IsValidIndex(Row) || !LogicalBoard[Row].IsValidIndex(Column) || LogicalBoard[Row][Column].Status == ETileStatus::REVEALED)
	{
		return 0;
	}
	
	
	if (LogicalBoard[Row][Column].Status == ETileStatus::MINE)
	{
		return 1;
	}
	
	LogicalBoard[Row][Column].Status = ETileStatus::REVEALED;
	RevealedTileCount+=1;
	
	int32 SurroundingMines = CountSurroundingMines(Row, Column);

	if (SurroundingMines == 0)
	{
		int32 x = RevealTile(Row-1, Column-1) + RevealTile(Row-1, Column) + RevealTile(Row-1, Column+1) + RevealTile(Row, Column-1) + RevealTile(Row, Column+1) + RevealTile(Row+1, Column-1) + RevealTile(Row+1, Column) + RevealTile(Row+1, Column+1);
	}
	LogicalBoard[Row][Column].SurroundingMines = SurroundingMines;
	OnTileRevealedDelegate.Broadcast(FTileCoordinate(Row, Column), LogicalBoard[Row][Column]);
	
	return SurroundingMines;
}

int32 FGameBoardModelData::CountSurroundingMines(int32 Row, int32 Column)
{
	int32 SurroundingMines = 0;
	for (int32 i = -1; i <= 1; i++)
	{
		for (int32 j = -1; j <= 1; j++)
		{
			if (Row == i && Column == j || !LogicalBoard.IsValidIndex(Row+i) || !LogicalBoard[Row+i].IsValidIndex(Column+j))
			{
				continue;
			}
			
			if (LogicalBoard[Row+i][Column+j].Status == ETileStatus::MINE)
			{
				SurroundingMines++;
			}
		}
	}
	
	return SurroundingMines;
}

bool FGameBoardModelData::SetMine(int32 InRow, int32 InColumn)
{
	if (!LogicalBoard.IsValidIndex(InRow) || !LogicalBoard[InRow].IsValidIndex(InColumn) || LogicalBoard[InRow][InColumn].Status == ETileStatus::MINE || LogicalBoard[InRow][InColumn].Status == ETileStatus::FIRST_OPEN)
	{
		return false;
	}
	
	if (LogicalBoard[InRow][InColumn].Status != ETileStatus::MINE && LogicalBoard[InRow][InColumn].Status != ETileStatus::FIRST_OPEN)
	{
		MinesCoords.Add(FTileCoordinate(InRow, InColumn));
		LogicalBoard[InRow][InColumn].Status = ETileStatus::MINE;
		return true;
	}else
	{
		for (int32 i = -1; i <= 1; i++)
		{
			for (int32 j = -1; j <= 1; j++)
			{
				bool bIsMinePlaced = SetMine(InRow+i, InColumn+j);
				if (bIsMinePlaced)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void FGameBoardModelData::SpawnMines(int32 DesireNumberOfMines, int32 ClickedRow, int32 ClickedColumn)
{
	int32 MinesSpawned = 0;
	LogicalBoard[ClickedRow][ClickedColumn].Status = ETileStatus::FIRST_OPEN;
	while (MinesSpawned < DesireNumberOfMines)
	{
		int32 RowMine = FMath::RandRange(0, WidthBoard - 1);
		int32 ColumnMine = FMath::RandRange(0, HeightBoard - 1);
		if (SetMine(RowMine, ColumnMine))
		{
			MinesSpawned++;
		}
	}
}
