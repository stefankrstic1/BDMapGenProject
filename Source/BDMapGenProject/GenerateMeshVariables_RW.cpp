// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateMeshVariables_RW.h"

UGenerateMeshVariables_RW* UGenerateMeshVariables_RW::MAKE(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, int _iterations, int _walkLength)
{
	UGenerateMeshVariables_RW* m = NewObject<UGenerateMeshVariables_RW>();
	m->init(_width, _height, _useRandomStream, _stream, _borderSize, _squareSize, _iterations, _walkLength);
	return m;
}

void UGenerateMeshVariables_RW::init(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, int _iterations, int _walkLength)
{
	Width = _width;
	Height = _height;
	UseRandomStream = _useRandomStream;
	Stream = _stream;
	BorderSize = _borderSize;
	SquareSize = _squareSize;
	WalkLength = _walkLength;
	Iterations = _iterations;

	StartPosition = FVector2D(Width / 2, Height / 2);

	Direction2D = UDirection2D::MAKE();

	TArray<int> temp;
	temp.Init(1, Height);
	Map.Init(temp, Width);
}

TArray<TArray<int>> UGenerateMeshVariables_RW::GenerateMapMatrix()
{
	RunProceduralGeneration();
	


	return Map;
}

TArray<FVector2D> UGenerateMeshVariables_RW::SimpleRandomWalk(FVector2D _startPosition, int _walkLength)
{
	TArray<FVector2D> path;
	bool bOutofMapBounds = false;

	path.Add(_startPosition);
	FVector2D previousPosition = _startPosition;
	for (int i = 0; i < _walkLength; i++)
	{
		FVector2D newPosition;
		do {
			newPosition = previousPosition + Direction2D->GetRandomCardinalDirection();
			if (newPosition.X >= Width || newPosition.X < 0 || newPosition.Y >= Height || newPosition.Y < 0)
				bOutofMapBounds = true;
			else
				bOutofMapBounds = false;
		} while (bOutofMapBounds);
		path.Add(newPosition);
		previousPosition = newPosition;
	}

	return path;
}

void UGenerateMeshVariables_RW::RunProceduralGeneration()
{
	WalkedPoints = RunRandomWalk();
	ChangeTilesToNotWall();
}

TArray<FVector2D> UGenerateMeshVariables_RW::RunRandomWalk()
{
	FVector2D currentPosition = StartPosition;
	TArray<FVector2D> floorPositions;
	for (int i = 0; i < Iterations; i++)
	{
		if (UseRandomStream)
			Stream.GenerateNewSeed();
		TArray<FVector2D> path = SimpleRandomWalk(currentPosition, WalkLength);
		floorPositions = MergeTwoArrays(floorPositions, path);
		//currentPosition = floorPositions[Stream.FRandRange(0, floorPositions.Num() - 1)];		
	}

	return floorPositions;
}

TArray<FVector2D> UGenerateMeshVariables_RW::MergeTwoArrays(TArray<FVector2D> first, TArray<FVector2D> second)
{	
	for (int i = 0; i < second.Num(); i++)
	{
		if (!first.Contains(second[i]))
		{
			first.Add(second[i]);
		}
	}

	return first;
}

void UGenerateMeshVariables_RW::ChangeTilesToNotWall()
{
	for (int i = 0; i < WalkedPoints.Num(); i++)
	{
		Map[WalkedPoints[i].X][WalkedPoints[i].Y] = 0;
	}
}
