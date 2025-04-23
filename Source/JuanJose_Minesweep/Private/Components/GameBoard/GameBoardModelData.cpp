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

void FGameBoardModelData::SelectTile(FTileCoordinate Coordinate)
{
	if (bIsFirstOpen == false)
	{
		bIsFirstOpen = true;
		SpawnMines(BoardMines, Coordinate);
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



int32 FGameBoardModelData::RevealTile(FTileCoordinate Coordinate)
{
	if (!IsValidTile({Coordinate.Row, Coordinate.Column}) || CheckTileStatus(Coordinate,ETileStatus::REVEALED))
	{
		return 0;
	}
	
	if (CheckTileStatus(Coordinate, ETileStatus::MINE))
	{
		return 1;
	}

	SetTileStatus(Coordinate, ETileStatus::REVEALED);
	RevealedTileCount+=1;
	
	int32 SurroundingMines = CountSurroundingMines(Coordinate);

	if (SurroundingMines == 0)
	{
		// If there are no surrounding mines, then reveal neighbor tiles
		for (int32 RowOffset = -1; RowOffset < WidthBoard; RowOffset++){
			for (int32 ColumnOffset = -1; ColumnOffset < HeightBoard; ColumnOffset++)
			{
				RevealTile({Coordinate.Row-RowOffset, Coordinate.Column-ColumnOffset});
			}
		}
	}
	SetTileSurroundingMines(Coordinate, SurroundingMines);
	OnTileRevealedDelegate.Broadcast(FTileCoordinate(Coordinate.Row, Coordinate.Column), LogicalBoard[Coordinate.Row][Coordinate.Column]);
	
	return SurroundingMines;
}

int32 FGameBoardModelData::CountSurroundingMines(FTileCoordinate Coordinate)
{
	int32 SurroundingMines = 0;
	for (int32 i = -1; i <= 1; i++)
	{
		for (int32 j = -1; j <= 1; j++)
		{
			if (!IsValidTile({Coordinate.Row+i, Coordinate.Column+j}))
			{
				continue;
			}
			
			if (CheckTileStatus({Coordinate.Row+i, Coordinate.Column+j}, ETileStatus::MINE))
			{
				SurroundingMines++;
			}
		}
	}
	
	return SurroundingMines;
}

void FGameBoardModelData::SpawnMines(int32 DesireNumberOfMines, FTileCoordinate OriginCoordinate)
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

bool FGameBoardModelData::PlaceMine(FTileCoordinate Coordinate)
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

void FGameBoardModelData::SetTileStatus(FTileCoordinate Coordinate, ETileStatus Status)
{
	LogicalBoard[Coordinate.Row][Coordinate.Column].Status = Status;
}

void FGameBoardModelData::SetTileSurroundingMines(FTileCoordinate Coordinate, int32 Mines)
{
	LogicalBoard[Coordinate.Row][Coordinate.Column].SurroundingMines = Mines;
}
