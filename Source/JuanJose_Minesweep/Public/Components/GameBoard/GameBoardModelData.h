#pragma once
#include "Resources/MinesweeperStructs.h"


struct FTileCoordinate;
enum class ETileStatus : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTileRevealedSignature, const FTileCoordinate /* TileCoord */, const FTileData /* Mines */);
DECLARE_MULTICAST_DELEGATE(FOnWinGameSignature);
DECLARE_MULTICAST_DELEGATE(FOnLoseGameSignature);

class FGameBoardModelData
{
	
public:
	FGameBoardModelData();
	
	void CreateLogicalBoard(int32 Width, int32 Height, int32 InNumberOfMines);
	void SelectTile(FTileCoordinate Coordinate);

	const TArray<FTileCoordinate>& GetMinesCoordinates() const;

	/** Checks if the given coordinate is inside the board */
	bool IsValidTile(FTileCoordinate Coordinate) const;
	/** Checks if the tile status specified in the coordinates is the same as the passed status */
	bool CheckTileStatus(FTileCoordinate Coordinate, ETileStatus Status) const;
	
	
public:
	FOnTileRevealedSignature OnTileRevealedDelegate;
	FOnWinGameSignature OnWinGameDelegate;
	FOnLoseGameSignature OnLoseGameDelegate;


	
private:
	int32 RevealTile(FTileCoordinate Coordinate);
	int32 CountSurroundingMines(FTileCoordinate Coordinate);

	/**
	 * Places mines all over the board. Generates the mines randomly around the tile origin, used after the first tile is revealed.
	 * @param DesireNumberOfMines Number of mines
	 * @param OriginCoordinate Origin tile coordinates 
	 */
	void SpawnMines(int32 DesireNumberOfMines, FTileCoordinate OriginCoordinate);
	bool PlaceMine(FTileCoordinate Coordinate);

	/** Sets the tile status */
	void SetTileStatus(FTileCoordinate Coordinate, ETileStatus Status);
	void SetTileSurroundingMines(FTileCoordinate Coordinate, int32 Mines);
	
private:
	TArray<TArray<FTileData>> LogicalBoard;
	TArray<FTileCoordinate> MinesCoords;

	int32 WidthBoard;
	int32 HeightBoard;
	int32 BoardMines;
	int32 RevealedTileCount;

	bool bIsFirstOpen;
};
