// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerateMeshVariablesParent.h"
#include "Direction2D.h"
#include "GenerateMeshVariables_RW.generated.h"

class UDirection2D;
/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UGenerateMeshVariables_RW : public UGenerateMeshVariablesParent
{
	GENERATED_BODY()

public:
	FVector2D StartPosition = FVector2D(0, 0);
	int Iterations = 10;
	int WalkLength = 10;
	bool StartRandomlyEachIteration = true;

	TArray<FVector2D> WalkedPoints;
	UDirection2D* Direction2D;

	static UGenerateMeshVariables_RW* MAKE(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, int _iterations, int _walkLength);
	void init(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, int _iterations, int _walkLength);
	
	TArray<TArray<int>> GenerateMapMatrix() override;

	TArray<FVector2D> SimpleRandomWalk(FVector2D startPosition, int walkLength);

	void RunProceduralGeneration();

	TArray<FVector2D> RunRandomWalk();
	TArray<FVector2D> MergeTwoArrays(TArray<FVector2D> first, TArray<FVector2D> second);

	void ChangeTilesToNotWall();
};
