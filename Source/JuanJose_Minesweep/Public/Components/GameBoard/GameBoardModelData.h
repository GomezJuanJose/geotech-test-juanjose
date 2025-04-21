#pragma once


struct FTileCoordinate;
enum ETileStatus : int32;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTileRevealedSignature, const FTileCoordinate /* TileCoord */, const ETileStatus /* Mines */);
DECLARE_MULTICAST_DELEGATE(FOnWinGameSignature);
DECLARE_MULTICAST_DELEGATE(FOnLoseGameSignature);

class FGameBoardModelData
{
public:
	FGameBoardModelData();
	
	TArray<TArray<ETileStatus>> CreateLogicalBoard(int32 InWidth, int32 InHeight, int32 InNumberOfMines);

	void SelectTile(int32 InRow, int32 InColumn);
	
public:
	FOnTileRevealedSignature OnTileRevealedDelegate;
	FOnWinGameSignature OnWinGameDelegate;
	FOnLoseGameSignature OnLoseGameDelegate;
	
private:
	int32 RevealTile(int32 Row, int32 Column);
	int32 CountSurroundingMines(int32 Row, int32 Column);
	
	void SpawnMines(int32 DesireNumberOfMines, int32 ClickedRow, int32 ClickedColumn);


	
private:
	TArray<TArray<ETileStatus>> LogicalBoard;
	TArray<FTileCoordinate> MinesCoords;

	int32 WidthBoard;
	int32 HeightBoard;
	int32 Mines;
	int32 RevealedTileCount;

	bool bIsFirstOpen;
};
