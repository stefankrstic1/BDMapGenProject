// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerateMeshVariablesParent.h"
#include "BSPNode.h"
#include "Math/UnrealMathUtility.h"
#include "GenerateMeshVariables_BSP.generated.h"

class UBSPNode;

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UGenerateMeshVariables_BSP : public UGenerateMeshVariablesParent
{
	GENERATED_BODY()

public:
	int NumberOfRooms = 6;
	int SmallestRegion;

	TArray<FVector2D> Corridors;

	UBSPNode* BSPRoot;

	static UGenerateMeshVariables_BSP* MAKE(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, int _numberOfRooms, int _smallestRegion);
	void init(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, int _numberOfRooms, int _smallestRegion);

	void SplitMapToRegions();
	TArray<TArray<int>> GenerateMapMatrix() override;
	void BSP(UBSPNode* Node, int NumberOfRooms);
	void CreateRoom(UBSPNode* Node);
	void Line(int x, int y, int x2, int y2);
	void AddCorridors();
	void AddRandomCorridors();

private:
	int ProbabilityDeltaDecider = 0;
};
