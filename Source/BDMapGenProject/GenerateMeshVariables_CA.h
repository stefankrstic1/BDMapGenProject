// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerateMeshVariablesParent.h"
#include "Math/RandomStream.h"
#include "Containers/Queue.h"
#include "Room.h"
#include "Math/Color.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenerateMeshVariables_CA.generated.h"

class URoom;

/**
 * 
 */
UCLASS(BlueprintType)
class BDMAPGENPROJECT_API UGenerateMeshVariables_CA : public UGenerateMeshVariablesParent
{
	GENERATED_BODY()
	
public:
	int RandomFillPercent;
	int CountOfMapSmoothing;

	static UGenerateMeshVariables_CA* MAKE(int _width, int _height, int _randomFillPercent, bool _useRandomSeed, FRandomStream _stream, int _countOfMapSmoothing, int _borderSize, float _squareSize);
	void init(int _width, int _height, int _randomFillPercent, bool _useRandomSeed, FRandomStream _stream, int _countOfMapSmoothing, int _borderSize, float _squareSize);

	TArray<FVector2D> GetRegionTiles(int startX, int startY);
	bool IsInMapRange(int x, int y);
	TArray<TArray<FVector2D>> GetRegions(int tileType);
	void ProcessMap();

	void ConnectClosestRooms(TArray<URoom*> allRooms, bool forceAccessibilityFromMainRoom = false);
	void CreatePassage(URoom* roomA, URoom* roomB, FVector2D tileA, FVector2D tileB);
	void DrawCircle(FVector2D c, int r);
	TArray<FVector2D> GetLine(FVector2D from, FVector2D to);

	FVector CoordToWorldPoint(FVector2D tile);

	TArray<TArray<int>> GenerateMapMatrix() override;

private:
	TArray<TArray<int>> Map;

	void RandomFillMap();
	void SmoothMap();
	int GetSurroundWallCount(int _gridX, int _gridY);
	TArray<TArray<int>> BorderMap(TArray<TArray<int>> _map);
};
