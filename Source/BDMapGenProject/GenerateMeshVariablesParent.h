// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapMeshVariables.h"
#include "MeshGeneration.h"
#include "GenerateMeshVariablesParent.generated.h"

class UMeshGeneration;

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UGenerateMeshVariablesParent : public UObject
{
	GENERATED_BODY()

public:
	TArray<TArray<int>> Map;
	UMeshGeneration* MeshGeneration;

	int Width = 80;
	int Height = 60;
	bool UseRandomStream = true;
	FRandomStream Stream;
	float SquareSize = 1;
	int BorderSize = 5;

	FMapMeshVariables GenerateMap();

	virtual TArray<TArray<int>> GenerateMapMatrix();
};
