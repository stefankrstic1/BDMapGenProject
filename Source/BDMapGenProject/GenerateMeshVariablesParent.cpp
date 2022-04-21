// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateMeshVariablesParent.h"


FMapMeshVariables UGenerateMeshVariablesParent::GenerateMap()
{
	Map = GenerateMapMatrix();
	MeshGeneration = UMeshGeneration::MAKE();

	return MeshGeneration->GenerateMesh(Map, SquareSize);
}

TArray<TArray<int>> UGenerateMeshVariablesParent::GenerateMapMatrix()
{
	return TArray<TArray<int>>();
}
