// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerateMeshVariablesParent.h"
#include "Room.h"
#include "GenerateMeshVariables_PN.generated.h"

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UGenerateMeshVariables_PN : public UGenerateMeshVariablesParent
{
	GENERATED_BODY()
	
public:
	float ThresholdValueWall = 0.4f;
	float Scale;
	int Octaves;
	float Persistance;
	float Lacunarity;
	FVector2D Offset;

	static UGenerateMeshVariables_PN* MAKE(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, float _thresholdValueWall, float _scale, int _octaves, float _persistance, float _lacunarity, FVector2D _offset);
	void init(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, float _thresholdValueWall, float _scale, int _octaves, float _persistance, float _lacunarity, FVector2D _offset);

	TArray<TArray<float>> GenerateNoiseMap();

	TArray<TArray<int>> NormalizeMap(TArray<TArray<float>> _floatMap);

	TArray<TArray<int>> GenerateMapMatrix() override;

	TArray<TArray<int>> BorderMap(TArray<TArray<int>> _map);

	void ProcessMap();

	void ConnectClosestRooms(TArray<URoom*> allRooms, bool forceAccessibilityFromMainRoom = false);
	void CreatePassage(URoom* roomA, URoom* roomB, FVector2D tileA, FVector2D tileB);
	void DrawCircle(FVector2D c, int r);
	TArray<FVector2D> GetLine(FVector2D from, FVector2D to);

	TArray<FVector2D> GetRegionTiles(int startX, int startY);
	TArray<TArray<FVector2D>> GetRegions(int tileType);

	bool IsInMapRange(int x, int y);

};
