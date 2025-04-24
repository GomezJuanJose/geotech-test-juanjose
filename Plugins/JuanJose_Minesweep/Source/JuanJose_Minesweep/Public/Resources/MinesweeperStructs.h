#pragma once
#include "MinesweeperEnums.h"
#include "MinesweeperStructs.generated.h"



USTRUCT()
struct FTileCoordinate
{
	GENERATED_BODY()
	
	int32 Row;
	int32 Column;

	FTileCoordinate() = default;

	FTileCoordinate(int32 InRow, int32 InColumn)
	{
		Row = InRow;
		Column = InColumn;
	}
};

USTRUCT()
struct FTileData
{
	GENERATED_BODY()
	
	int32 SurroundingMines;
	ETileStatus Status;

	FTileData() = default;

	FTileData(int32 InSurroundingMines, ETileStatus InStatus)
	{
		SurroundingMines = InSurroundingMines;
		Status = InStatus;
	}
};