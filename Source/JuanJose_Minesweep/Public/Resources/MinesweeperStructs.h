#pragma once

USTRUCT()
struct FTileCoordinate
{
	GENERATED_USTRUCT_BODY()
	int32 Row;
	int32 Column;

	FTileCoordinate(int32 InRow, int32 InColumn)
	{
		Row = InRow;
		Column = InColumn;
	}
};