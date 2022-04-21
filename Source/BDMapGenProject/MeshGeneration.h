// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapMeshVariables.h"
#include "Square.h"
#include "SquareGrid.h"
#include "TriangleClass.h"
#include "Containers/Set.h"
#include "Kismet/KismetMathLibrary.h"
#include "MeshGeneration.generated.h"

class USquareGrid;
class USquare;
class UTriangleClass;

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UMeshGeneration : public UObject
{
	GENERATED_BODY()

public:
	USquareGrid* SquareGrid;
	TArray<TArray<USquare*>> Squares;

	float SquareSize = 1;

	FMapMeshVariables MapMeshVariables;
	
	FMapMeshVariables GenerateMesh(TArray<TArray<int>> _map, float _squareSize);

	static UMeshGeneration* MAKE();

	TMap<int, TArray<UTriangleClass*>> TriangleDictionary;
	TArray<TArray<int>> Outlines;
	TSet<int> CheckedVertices;

	void TriangulateSquare(USquare* square);

	void MeshFromPoints(TArray<UVertexNode*> points);

	void AssignVertices(TArray<UVertexNode*> points);

	void CreateTriangle(UVertexNode* a, UVertexNode* b, UVertexNode* c);

	void AddTriangleToDictionary(int vertexIndexKey, UTriangleClass* _Triangle);

	bool IsOutlineEdge(int _vertexA, int _vertexB);

	int GetConnectedOutlineVertex(int _vertexIndex);

	void CalculateMeshOutlines();

	void FollowOutline(int vertexIndex, int outlineIndex);

	void CreateWallMesh();
};
