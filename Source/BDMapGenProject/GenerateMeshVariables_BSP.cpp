// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateMeshVariables_BSP.h"

UGenerateMeshVariables_BSP* UGenerateMeshVariables_BSP::MAKE(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, int _numberOfRooms, int _smallestRegion)
{
	UGenerateMeshVariables_BSP* m = NewObject<UGenerateMeshVariables_BSP>();
	m->init(_width, _height, _useRandomStream, _stream, _borderSize, _squareSize, _numberOfRooms, _smallestRegion);
	return m;
}

void UGenerateMeshVariables_BSP::init(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, int _numberOfRooms, int _smallestRegion)
{
	Width = _width;
	Height = _height;
	UseRandomStream = _useRandomStream;
	Stream = _stream;
	BorderSize = _borderSize;
	SquareSize = _squareSize;
	NumberOfRooms = _numberOfRooms;

	if (_smallestRegion < 10) 
	{
		_smallestRegion = 10;
	}

	SmallestRegion = _smallestRegion;

	// Inicijalizujemo Mapu da sve vrednosti budu 1
	TArray<int> temp;
	temp.Init(1, Height);
	Map.Init(temp, Width);
}

void UGenerateMeshVariables_BSP::SplitMapToRegions()
{
	int CurrentRooms = 1;
	BSPRoot = UBSPNode::MAKE(FVector2D(0,0), Width, Height);
	BSP(BSPRoot, NumberOfRooms);
}

TArray<TArray<int>> UGenerateMeshVariables_BSP::GenerateMapMatrix()
{
	SplitMapToRegions();
	AddCorridors();

	return Map;
}

void UGenerateMeshVariables_BSP::AddCorridors()
{
	for (int i = 1; i < Corridors.Num(); i++)
	{
		if ((int)Corridors[i].X == (int)Corridors[i - 1].X || (int)Corridors[i].Y == (int)Corridors[i - 1].Y)
		{
			Line((int)Corridors[i].X, (int)Corridors[i].Y, (int)Corridors[i - 1].X, (int)Corridors[i - 1].Y);
		}
		else
		{
			Line((int)Corridors[i].X, (int)Corridors[i].Y, (int)Corridors[i].X, (int)Corridors[i - 1].Y);
			Line((int)Corridors[i].X, (int)Corridors[i].Y, (int)Corridors[i - 1].X, (int)Corridors[i].Y);
		}
	}
}

void UGenerateMeshVariables_BSP::BSP(UBSPNode* Node, int _numberOfRooms)
{
	if (Node == nullptr)
		return;

	if (_numberOfRooms <= 0)
	{
		CreateRoom(Node);
		Corridors.Add(FVector2D(Node->Point.X + Node->Width / 2, Node->Point.Y + Node->Height / 2));
		return;
	}
	
	if (Node->Split(Stream, SmallestRegion))
	{
		BSP(Node->First, _numberOfRooms - 1);
		BSP(Node->Second, _numberOfRooms - 1);
	}
	else
	{
		CreateRoom(Node);
		Corridors.Add(FVector2D(Node->Point.X + Node->Width / 2, Node->Point.Y + Node->Height / 2));
	}

}

void UGenerateMeshVariables_BSP::CreateRoom(UBSPNode* Node)
{
	int wallSize = Stream.RandRange(2, 4);

	for (int x = Node->Point.X + wallSize; x < Node->Width + Node->Point.X - wallSize; x++)
	{
		for (int y = Node->Point.Y + wallSize; y < Node->Height + Node->Point.Y - wallSize; y++)
		{
			Map[x][y] = 0;
		}
	}
}

void UGenerateMeshVariables_BSP::Line(int x, int y, int x2, int y2)
{
	int w = x2 - x;
	int h = y2 - y;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	if (w < 0) dx2 = -1; else if (w > 0) dx2 = 1;
	int longest = FMath::Abs(w);
	int shortest = FMath::Abs(h);
	if (!(longest > shortest))
	{
		longest = FMath::Abs(h);
		shortest = FMath::Abs(w);
		if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
		dx2 = 0;
	}
	int gradientAccumulation = longest / 1;
	for (int i = 0; i <= longest; i++)
	{
		Map[x][y] = 0;
		gradientAccumulation += shortest;
		if (!(gradientAccumulation < longest))
		{
			gradientAccumulation -= longest;
			x += dx1;
			y += dy1;
		}
		else
		{
			x += dx2;
			y += dy2;
		}
	}
}
