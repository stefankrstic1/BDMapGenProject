// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareGrid.h"

USquareGrid* USquareGrid::MAKE()
{
	USquareGrid* s = NewObject<USquareGrid>();
	return s;
}

TArray<TArray<USquare*>> USquareGrid::GenerateSquareGrid(TArray<TArray<int>> _map, float _squareSize)
{
	int NodeCountX = _map.Num();

	int NodeCountY = _map[0].Num();
	float mapWidth = NodeCountX * _squareSize;
	float mapHeight = NodeCountY * _squareSize;

	TArray<TArray<UControlVertexNode*>> ControlVertexNodeMatrix;
	

	for (int x = 0; x < NodeCountX; x++)
	{
		TArray<UControlVertexNode*> ControlVertexNodeArray;
		for (int y = 0; y < NodeCountY; y++)
		{
			FVector Position = FVector(-mapHeight / 2 + y * _squareSize + _squareSize / 2, -mapWidth / 2 + x * _squareSize + _squareSize / 2, 0);
			ControlVertexNodeArray.Add(UControlVertexNode::MAKE(Position, _map[x][y] == 1, _squareSize));
		}
		ControlVertexNodeMatrix.Add(ControlVertexNodeArray);
		ControlVertexNodeArray.Empty();
	}

	for (int x = 0; x < NodeCountX - 1; x++)
	{
		TArray<USquare*> SquareArray;
		for (int y = 0; y < NodeCountY - 1; y++)
		{
			SquareArray.Add(USquare::MAKE(ControlVertexNodeMatrix[x][y + 1], ControlVertexNodeMatrix[x + 1][y + 1], ControlVertexNodeMatrix[x + 1][y], ControlVertexNodeMatrix[x][y]));
		}
		Squares.Add(SquareArray);
		SquareArray.Empty();
	}

	return Squares;

}
