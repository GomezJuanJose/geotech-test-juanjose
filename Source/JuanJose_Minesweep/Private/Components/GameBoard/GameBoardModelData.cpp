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

TArray<TArray<ETileStatus>> FGameBoardModelData::CreateLogicalBoard(int32 InWidth, int32 InHeight, int32 InNumberOfMines)
{
	LogicalBoard.Empty();
	MinesCoords.Empty();
	WidthBoard = InWidth;
	HeightBoard = InHeight;
	Mines = InNumberOfMines;
	bIsFirstOpen = false;
	
	for (int32 Row = 0; Row < InWidth; Row++)
	{
		TArray<ETileStatus> TileRow;
		TileRow.Init(ETileStatus::NONE, InHeight);
		LogicalBoard.Add(TileRow);
	}
	
	return LogicalBoard;
}

void FGameBoardModelData::SelectTile(int32 InRow, int32 InColumn)
{
	if (bIsFirstOpen == false)
	{
		bIsFirstOpen = true;
		SpawnMines(Mines, InRow, InColumn);
	}
				
	if (LogicalBoard[InRow][InColumn] == ETileStatus::MINE) // Never hits the condition on the first click because the mines are spawned after the click
	{
		OnLoseGameDelegate.Broadcast();
	}
		
	RevealTile(InRow, InColumn);

	if (RevealedTileCount == (WidthBoard * HeightBoard) - Mines)
	{
		OnWinGameDelegate.Broadcast();
	}
}

int32 FGameBoardModelData::RevealTile(int32 Row, int32 Column)
{
	if (!LogicalBoard.IsValidIndex(Row) || !LogicalBoard[Row].IsValidIndex(Column) || LogicalBoard[Row][Column] >= ETileStatus::REVEALED)
	{
		return 0;
	}
	
	
	if (LogicalBoard[Row][Column] == ETileStatus::MINE)
	{
		return 1;
	}
	
	LogicalBoard[Row][Column] = ETileStatus::REVEALED;
	RevealedTileCount+=1;
	
	ETileStatus SurroundingMines = (ETileStatus)CountSurroundingMines(Row, Column);

	if (SurroundingMines == 0)
	{
		int32 x = RevealTile(Row-1, Column-1) + RevealTile(Row-1, Column) + RevealTile(Row-1, Column+1) + RevealTile(Row, Column-1) + RevealTile(Row, Column+1) + RevealTile(Row+1, Column-1) + RevealTile(Row+1, Column) + RevealTile(Row+1, Column+1);
	}
	LogicalBoard[Row][Column] = SurroundingMines;
	OnTileRevealedDelegate.Broadcast(FTileCoordinate(Row, Column), SurroundingMines);
	
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
			
			if (LogicalBoard[Row+i][Column+j] == ETileStatus::MINE)
			{
				SurroundingMines++;
			}
		}
	}
	
	return SurroundingMines;
}

void FGameBoardModelData::SpawnMines(int32 DesireNumberOfMines, int32 ClickedRow, int32 ClickedColumn)
{
	int32 MinesSpawned = 0;
	while (MinesSpawned < DesireNumberOfMines)
	{
		int32 RowMine = FMath::RandRange(0, (int32)WidthBoard - 1);
		int32 ColumnMine = FMath::RandRange(0, (int32) HeightBoard - 1);
							
		if (LogicalBoard[RowMine][ColumnMine] != ETileStatus::MINE && RowMine != ClickedRow && ColumnMine != ClickedColumn)
		{
			MinesCoords.Add(FTileCoordinate(RowMine, ColumnMine));
			LogicalBoard[RowMine][ColumnMine] = ETileStatus::MINE;
			MinesSpawned++;
		}
	}
}
