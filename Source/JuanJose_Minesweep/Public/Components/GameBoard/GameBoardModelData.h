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
	void SelectTile(const FTileCoordinate Coordinate);

	const TArray<FTileCoordinate>& GetMinesCoordinates() const;

	/** Checks if the given coordinate is inside the board */
	bool IsValidTile(const FTileCoordinate Coordinate) const;
	/** Checks if the tile status specified in the coordinates is the same as the passed status */
	bool CheckTileStatus(const FTileCoordinate Coordinate, ETileStatus Status) const;
	
	
public:
	FOnTileRevealedSignature OnTileRevealedDelegate;
	FOnWinGameSignature OnWinGameDelegate;
	FOnLoseGameSignature OnLoseGameDelegate;


	
private:
	void RevealTile(const FTileCoordinate Coordinate);
	int32 CountSurroundingMines(const FTileCoordinate Coordinate);

	/**
	 * Places mines all over the board. Generates the mines randomly around the tile origin, used after the first tile is revealed.
	 * @param DesireNumberOfMines Number of mines
	 * @param OriginCoordinate Origin tile coordinates 
	 */
	void SpawnMines(const FTileCoordinate OriginCoordinate, int32 DesireNumberOfMines);
	bool PlaceMine(const FTileCoordinate Coordinate);

	/** Sets the tile status */
	void SetTileStatus(const FTileCoordinate Coordinate, ETileStatus Status);
	void SetTileSurroundingMines(const FTileCoordinate Coordinate, int32 Mines);
	
private:
	TArray<TArray<FTileData>> LogicalBoard;
	TArray<FTileCoordinate> MinesCoords;

	int32 WidthBoard;
	int32 HeightBoard;
	int32 BoardMines;
	int32 RevealedTileCount;

	bool bIsFirstOpen;
};
