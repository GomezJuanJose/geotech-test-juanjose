#pragma once
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