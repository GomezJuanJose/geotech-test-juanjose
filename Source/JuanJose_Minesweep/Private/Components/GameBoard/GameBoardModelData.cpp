#include "Components/GameBoard/GameBoardModelData.h"
#include "Resources/MinesweeperEnums.h"
#include "Resources/MinesweeperStructs.h"

FGameBoardModelData::FGameBoardModelData()
{
	WidthBoard = 0;
	HeightBoard = 0;
	BoardMines = 0;
	bIsFirstOpen = false;
}

void FGameBoardModelData::CreateLogicalBoard(int32 Width, int32 Height, int32 InNumberOfMines)
{
	LogicalBoard.Empty();
	MinesCoords.Empty();

	WidthBoard = Width;
	HeightBoard = Height;
	BoardMines = InNumberOfMines;
	RevealedTileCount = 0;
	bIsFirstOpen = false;
	
	for (int32 Row = 0; Row < Width; Row++)
	{
		TArray<FTileData> TileRow;
		TileRow.Init({-1, ETileStatus::NONE}, Height);
		LogicalBoard.Add(TileRow);
	}
}

void FGameBoardModelData::SelectTile(const FTileCoordinate Coordinate)
{
	if (bIsFirstOpen == false)
	{
		bIsFirstOpen = true;
		SpawnMines(Coordinate, BoardMines);
	}

	// Never hits the condition on the first click because the mines are spawned after the click
	if (CheckTileStatus(Coordinate, ETileStatus::MINE)) 
	{
		OnLoseGameDelegate.Broadcast();
		return;
	}
		
	RevealTile(Coordinate);

	// Only if all the non-bomb tiles are revealed
	if (RevealedTileCount == (WidthBoard * HeightBoard) - BoardMines)
	{
		OnWinGameDelegate.Broadcast();
	}
}



void FGameBoardModelData::RevealTile(const FTileCoordinate Coordinate)
{
	if (!IsValidTile({Coordinate.Row, Coordinate.Column}) || CheckTileStatus(Coordinate, ETileStatus::REVEALED) || CheckTileStatus(Coordinate, ETileStatus::MINE))
	{
		return;
	}

	SetTileStatus(Coordinate, ETileStatus::REVEALED);
	RevealedTileCount+=1;
	
	int32 SurroundingMines = CountSurroundingMines(Coordinate);

	if (SurroundingMines == 0)
	{
		// If there are no surrounding mines, then reveal neighbor tiles
		for (int32 RowOffset = -1; RowOffset <= 1; RowOffset++){
			for (int32 ColumnOffset = -1; ColumnOffset <= 1; ColumnOffset++)
			{
				RevealTile({Coordinate.Row-RowOffset, Coordinate.Column-ColumnOffset});
			}
		}
	}
	SetTileSurroundingMines(Coordinate, SurroundingMines);
	OnTileRevealedDelegate.Broadcast(Coordinate, LogicalBoard[Coordinate.Row][Coordinate.Column]);
}

int32 FGameBoardModelData::CountSurroundingMines(const FTileCoordinate Coordinate)
{
	int32 SurroundingMines = 0;
	for (int32 RowOffset = -1; RowOffset <= 1; RowOffset++)
	{
		for (int32 ColumnOffset = -1; ColumnOffset <= 1; ColumnOffset++)
		{
			if (!IsValidTile({Coordinate.Row+RowOffset, Coordinate.Column+ColumnOffset}))
			{
				continue;
			}
			
			if (CheckTileStatus({Coordinate.Row+RowOffset, Coordinate.Column+ColumnOffset}, ETileStatus::MINE))
			{
				SurroundingMines++;
			}
		}
	}
	
	return SurroundingMines;
}

void FGameBoardModelData::SpawnMines(const FTileCoordinate OriginCoordinate, int32 DesireNumberOfMines)
{
	int32 MinesSpawned = 0;
	SetTileStatus(OriginCoordinate, ETileStatus::FIRST_OPEN);
	while (MinesSpawned < DesireNumberOfMines)
	{
		int32 RandomRowMine = FMath::RandRange(0, WidthBoard - 1);
		int32 RandomColumnMine = FMath::RandRange(0, HeightBoard - 1);
		if (PlaceMine({RandomRowMine, RandomColumnMine}))
		{
			MinesSpawned++;
		}
	}
}

bool FGameBoardModelData::PlaceMine(const FTileCoordinate Coordinate)
{
	if (!IsValidTile(Coordinate) || CheckTileStatus(Coordinate, ETileStatus::MINE) || CheckTileStatus(Coordinate, ETileStatus::FIRST_OPEN))
	{
		return false;
	}
	
	if (!CheckTileStatus(Coordinate, ETileStatus::MINE) && !CheckTileStatus(Coordinate, ETileStatus::FIRST_OPEN))
	{
		MinesCoords.Add(FTileCoordinate(Coordinate.Row, Coordinate.Column));
		SetTileStatus(Coordinate, ETileStatus::MINE);
		return true;
	}else
	{
		//In case that the coordinates were not valid, then start to search recursively in the nearest neighbors to find a valid one.
		for (int32 i = -1; i <= 1; i++)
		{
			for (int32 j = -1; j <= 1; j++)
			{
				bool bIsMinePlaced = PlaceMine({Coordinate.Row+i, Coordinate.Column+j});
				if (bIsMinePlaced)
				{
					return true;
				}
			}
		}
	}
	return false;
}


const TArray<FTileCoordinate>& FGameBoardModelData::GetMinesCoordinates() const
{
	return MinesCoords;
}

bool FGameBoardModelData::IsValidTile(FTileCoordinate Coordinate) const
{
	return LogicalBoard.IsValidIndex(Coordinate.Row) && LogicalBoard[Coordinate.Row].IsValidIndex(Coordinate.Column);
}

bool FGameBoardModelData::CheckTileStatus(FTileCoordinate Coordinate, ETileStatus Status) const
{
	return LogicalBoard[Coordinate.Row][Coordinate.Column].Status == Status;
}

void FGameBoardModelData::SetTileStatus(const FTileCoordinate Coordinate, ETileStatus Status)
{
	LogicalBoard[Coordinate.Row][Coordinate.Column].Status = Status;
}

void FGameBoardModelData::SetTileSurroundingMines(const FTileCoordinate Coordinate, int32 Mines)
{
	LogicalBoard[Coordinate.Row][Coordinate.Column].SurroundingMines = Mines;
}
