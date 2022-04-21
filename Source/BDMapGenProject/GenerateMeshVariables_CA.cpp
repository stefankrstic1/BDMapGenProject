// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateMeshVariables_CA.h"

UGenerateMeshVariables_CA* UGenerateMeshVariables_CA::MAKE(int _width, int _height, int _randomFillPercent, bool _useRandomSeed, FRandomStream _stream, int _countOfMapSmoothing, int _borderSize, float _squareSize)
{
	UGenerateMeshVariables_CA* m = NewObject<UGenerateMeshVariables_CA>();
	m->init(_width, _height, _randomFillPercent, _useRandomSeed, _stream, _countOfMapSmoothing, _borderSize, _squareSize);
	return m;
}

void UGenerateMeshVariables_CA::init(int _width, int _height, int _randomFillPercent, bool _useRandomSeed, FRandomStream _stream, int _countOfMapSmoothing, int _borderSize, float _squareSize)
{
	Width = _width;
	Height = _height;
	RandomFillPercent = _randomFillPercent;
	UseRandomStream = _useRandomSeed;
	Stream = _stream;
	CountOfMapSmoothing = _countOfMapSmoothing;
	BorderSize = _borderSize;
	SquareSize = _squareSize;
}

TArray<FVector2D> UGenerateMeshVariables_CA::GetRegionTiles(int startX, int startY)
{
	TArray<FVector2D> tiles;
	TArray<TArray<int>> mapFlags;
	int tileType = Map[startX][startY];

	TQueue<FVector2D> queue;
	queue.Enqueue(FVector2D(startX, startY));

	for (int x = 0; x < Width; x++)
	{
		TArray<int> mapFlag;
		for (int y = 0; y < Height; y++)
		{
			mapFlag.Add(0);
		}
		mapFlags.Add(mapFlag);
		mapFlag.Empty();
	}

	mapFlags[startX][startY] = 1;

	while (!queue.IsEmpty())
	{
		FVector2D tile;
		queue.Dequeue(tile);
		tiles.Add(tile);

		for (int x = tile.X - 1; x <= tile.X + 1; x++)
		{
			for (int y = tile.Y - 1; y <= tile.Y + 1; y++)
			{
				if (IsInMapRange(x, y) && (y == tile.Y || x == tile.X))
				{
					if (mapFlags[x][y] == 0 && Map[x][y] == tileType)
					{
						mapFlags[x][y] = 1;
						queue.Enqueue(FVector2D(x, y));
					}
				}
			}
		}
	}

	return tiles;
}

bool UGenerateMeshVariables_CA::IsInMapRange(int x, int y)
{
	return x >= 0 && x < Width&& y >= 0 && y < Height;
}

TArray<TArray<FVector2D>> UGenerateMeshVariables_CA::GetRegions(int tileType)
{
	TArray<TArray<FVector2D>> regions;
	TArray<TArray<int>> mapFlags;

	for (int x = 0; x < Width; x++)
	{
		TArray<int> mapFlag;
		for (int y = 0; y < Height; y++)
		{
			mapFlag.Add(0);
		}
		mapFlags.Add(mapFlag);
		mapFlag.Empty();
	}

	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			if (mapFlags[x][y] == 0 && Map[x][y] == tileType)
			{
				TArray<FVector2D> newRegion = GetRegionTiles(x, y);
				regions.Add(newRegion);

				for (FVector2D tile : newRegion)
				{
					mapFlags[tile.X][tile.Y] = 1;
				}
			}
		}
	}

	return regions;
}

void UGenerateMeshVariables_CA::ProcessMap()
{
	TArray<TArray<FVector2D>> wallRegions = GetRegions(1);
	int wallThresholdSize = 50;
	for (TArray<FVector2D> wallRegion : wallRegions)
	{
		if (wallRegion.Num() < wallThresholdSize)
		{
			for (FVector2D tile : wallRegion)
			{
				Map[tile.X][tile.Y] = 0;
			}
		}
	}

	TArray<TArray<FVector2D>> roomRegions = GetRegions(0);
	int roomThresholdSize = 50;
	TArray<URoom*> survivingRooms;


	for (TArray<FVector2D> roomRegion : roomRegions)
	{
		if (roomRegion.Num() < roomThresholdSize)
		{
			for (FVector2D tile : roomRegion)
			{
				Map[tile.X][tile.Y] = 1;
			}
		}
		else
		{
			survivingRooms.Add(URoom::MAKE(roomRegion, Map));
		}
	}

	Algo::SortBy(survivingRooms, &URoom::roomSize, TGreater<>());

	survivingRooms[0]->bIsMainRoom = true;
	survivingRooms[0]->bIsAccessibleFromMainRoom = true;

	ConnectClosestRooms(survivingRooms);
}

void UGenerateMeshVariables_CA::CreatePassage(URoom* roomA, URoom* roomB, FVector2D tileA, FVector2D tileB)
{
	URoom::ConnectRooms(roomA, roomB);

	TArray<FVector2D> line = GetLine(tileA, tileB);

	for (FVector2D c : line)
	{
		DrawCircle(c, FMath::RandRange(1, 3));
	}
}

void UGenerateMeshVariables_CA::DrawCircle(FVector2D c, int r)
{
	for (int x = -r; x <= r; x++)
	{
		for (int y = -r; y < r; y++)
		{
			if (x * x + y * y <= r * r)
			{
				int realX = c.X + x;
				int realY = c.Y + y;
				if (IsInMapRange(realX, realY))
				{
					Map[realX][realY] = 0;
				}
			}
		}
	}
}

TArray<FVector2D> UGenerateMeshVariables_CA::GetLine(FVector2D from, FVector2D to)
{
	/*TArray<FCoord> line;

	int x = from.tileX;
	int y = from.tileY;

	int dx = to.tileX - from.tileX;
	int dy = to.tileY - from.tileY;

	bool inverted = false;

	int step = UKismetMathLibrary::SignOfInteger(dx);
	int gradientStep = UKismetMathLibrary::SignOfInteger(dy);

	int longest = UKismetMathLibrary::Abs(dx);
	int shortest = UKismetMathLibrary::Abs(dy);

	if (longest < shortest)
	{
		inverted = true;
		longest = UKismetMathLibrary::Abs(dy);
		shortest = UKismetMathLibrary::Abs(dx);

		step = UKismetMathLibrary::SignOfInteger(dy);
		gradientStep = UKismetMathLibrary::SignOfInteger(dx);
	}

	int gradientAccumulation = longest / 2;
	for (int i = 0; i < longest; i++)
	{
		FCoord newCoord;
		newCoord.tileX = x;
		newCoord.tileY = y;

		line.Add(newCoord);

		if (inverted)
			y += step;
		else
			x += step;

		gradientAccumulation += shortest;
		if (gradientAccumulation >= longest)
		{
			if (inverted)
			{
				x += gradientStep;
			}
			else
			{
				y += gradientStep;
			}
			gradientAccumulation -= longest;
		}
	}

	return line;
	*/
	int x = from.X;
	int y = from.Y;

	TArray<FVector2D> line;

	int w = to.X - from.X;
	int h = to.Y - from.Y;
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
	int gradientAccumulation = longest / 2;
	for (int i = 0; i <= longest; i++)
	{
		line.Add(FVector2D(x,y));
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

	return line;
}

FVector UGenerateMeshVariables_CA::CoordToWorldPoint(FVector2D tile)
{
	FVector vector = FVector(-Height / 2 + .5f + tile.Y, -Width / 2 + .5f + tile.X, 2);
	return vector;
}

TArray<TArray<int>> UGenerateMeshVariables_CA::GenerateMapMatrix()
{
	RandomFillMap();

	for (int i = 0; i < CountOfMapSmoothing; i++)
	{
		SmoothMap();
	}

	ProcessMap();

	//Call borderMap

	Map = BorderMap(Map);

	return Map;
}

void UGenerateMeshVariables_CA::RandomFillMap()
{
	if (UseRandomStream)
		Stream.GenerateNewSeed();


	for (int x = 0; x < Width; x++)
	{
		TArray<int> IntArray;
		int Value = 0;
		for (int y = 0; y < Height; y++)
		{
			if ((x == 0) || (x == Width - 1) || (y == 0) || (y == Height - 1))
			{
				Value = 1;
			}
			else
			{
				Value = (Stream.RandRange(0, 100) < RandomFillPercent) ? 1 : 0;
			}

			IntArray.Add(Value);
		}
		Map.Add(IntArray);
		IntArray.Empty();
	}
}

void UGenerateMeshVariables_CA::SmoothMap()
{
	TArray<TArray<int>> MapTemp = Map;

	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			int NeighbourWallTiles = GetSurroundWallCount(x, y);

			if (NeighbourWallTiles > 4)
				MapTemp[x][y] = 1;
			else if (NeighbourWallTiles < 4)
				MapTemp[x][y] = 0;
		}
	}

	Map = MapTemp;
}

int UGenerateMeshVariables_CA::GetSurroundWallCount(int _gridX, int _gridY)
{
	int wallCount = 0;
	for (int neighbourX = _gridX - 1; neighbourX <= _gridX + 1; neighbourX++)
	{
		for (int neighbourY = _gridY - 1; neighbourY <= _gridY + 1; neighbourY++)
		{
			if (IsInMapRange(neighbourX, neighbourY))
			{
				if (neighbourX != _gridX || neighbourY != _gridY)
				{
					wallCount += Map[neighbourX][neighbourY];
				}
			}
			else
			{
				wallCount++;
			}
		}
	}
	return wallCount;
}

TArray<TArray<int>> UGenerateMeshVariables_CA::BorderMap(TArray<TArray<int>> _map)
{
	TArray<TArray<int>> borderedMap;

	for (int x = 0; x < Width + BorderSize * 2; x++)
	{
		TArray<int> IntArray;
		for (int y = 0; y < Height + BorderSize * 2; y++)
		{
			if (x >= BorderSize && x < Width + BorderSize && y >= BorderSize && y < Height + BorderSize) {
				IntArray.Add(_map[x - BorderSize][y - BorderSize]);
			}
			else {
				IntArray.Add(1);
			}
		}
		borderedMap.Add(IntArray);
		IntArray.Empty();
	}

	return borderedMap;
}

void UGenerateMeshVariables_CA::ConnectClosestRooms(TArray<URoom*> allRooms, bool forceAccessibilityFromMainRoom)
{
	TArray<URoom*> roomListA;
	TArray<URoom*> roomListB;

	if (forceAccessibilityFromMainRoom)
	{
		for (URoom* room : allRooms)
		{
			if (room->bIsAccessibleFromMainRoom)
			{
				roomListB.Add(room);
			}
			else
			{
				roomListA.Add(room);
			}
		}
	}
	else
	{
		roomListA = allRooms;
		roomListB = allRooms;
	}

	int bestDistance = 0;
	FVector2D bestTileA;
	FVector2D bestTileB;

	// Bolje ovo inicijalizuj
	URoom* bestRoomA = URoom::MAKE(TArray<FVector2D>(), Map);
	URoom* bestRoomB = URoom::MAKE(TArray<FVector2D>(), Map);
	bool possibleConnectionFound = false;

	for (URoom* roomA : roomListA)
	{
		if (!forceAccessibilityFromMainRoom)
		{
			possibleConnectionFound = false;
			if (roomA->connectedRooms.Num())
			{
				continue;
			}
		}

		for (URoom* roomB : roomListB)
		{
			if (roomA == roomB || roomA->IsConnected(roomB))
				continue;

			for (int tileIndexA = 0; tileIndexA < roomA->edgeTiles.Num(); tileIndexA++)
			{
				for (int tileIndexB = 0; tileIndexB < roomB->edgeTiles.Num(); tileIndexB++)
				{
					FVector2D tileA = roomA->edgeTiles[tileIndexA];
					FVector2D tileB = roomB->edgeTiles[tileIndexB];
					int distanceBetweenRooms = (int)FGenericPlatformMath::Pow(tileA.X - tileB.X, 2) + FGenericPlatformMath::Pow(tileA.Y - tileB.Y, 2);

					if (distanceBetweenRooms < bestDistance || !possibleConnectionFound)
					{
						bestDistance = distanceBetweenRooms;
						possibleConnectionFound = true;
						bestTileA = tileA;
						bestTileB = tileB;
						bestRoomA = roomA;
						bestRoomB = roomB;
					}
				}
			}
		}

		if (possibleConnectionFound && !forceAccessibilityFromMainRoom)
		{
			CreatePassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
		}
	}

	if (possibleConnectionFound && forceAccessibilityFromMainRoom)
	{
		CreatePassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
		ConnectClosestRooms(allRooms, true);
	}

	if (!forceAccessibilityFromMainRoom)
	{
		ConnectClosestRooms(allRooms, true);
	}
}
