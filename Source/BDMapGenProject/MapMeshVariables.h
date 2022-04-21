// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MapMeshVariables.generated.h"

USTRUCT(BlueprintType)
struct FMapMeshVariables
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadWrite)
	TArray<int> Triangles;

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> WallVertices;

	UPROPERTY(BlueprintReadWrite)
	TArray<int> WallTriangles;

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector2D> WallUVs;

};
