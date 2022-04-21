// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateMeshVariables_PN.h"

UGenerateMeshVariables_PN* UGenerateMeshVariables_PN::MAKE(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, float _thresholdValueWall, float _scale, int _octaves, float _persistance, float _lacunarity, FVector2D _offset)
{
	UGenerateMeshVariables_PN* m = NewObject<UGenerateMeshVariables_PN>();
	m->init(_width, _height, _useRandomStream, _stream, _borderSize, _squareSize, _thresholdValueWall, _scale, _octaves, _persistance, _lacunarity, _offset);
	return m;
}

void UGenerateMeshVariables_PN::init(int _width, int _height, bool _useRandomStream, FRandomStream _stream, int _borderSize, float _squareSize, float _thresholdValueWall, float _scale, int _octaves, float _persistance, float _lacunarity, FVector2D _offset)
{
	Width = _width;
	Height = _height;
	UseRandomStream = _useRandomStream;
	Stream = _stream;
	BorderSize = _borderSize;
	SquareSize = _squareSize;
	ThresholdValueWall = _thresholdValueWall;
	Scale = _scale;
	Octaves = _octaves;
	Persistance = _persistance;
	Lacunarity = _lacunarity;
	Offset = _offset;
}

TArray<TArray<float>> UGenerateMeshVariables_PN::GenerateNoiseMap()
{
	TArray<TArray<float>> NoiseMap;
	TArray<float> temp;

	temp.Init(-1, Height);
	NoiseMap.Init(temp, Width);

	if (UseRandomStream)
	{
		Stream.GenerateNewSeed();
	}

	TArray<FVector2D> octaveOffsets;
	for (int i = 0; i < Octaves; i++)
	{
		float offsetX = Stream.FRandRange(-100000, 100000) + Offset.X;
		float offsetY = Stream.FRandRange(-100000, 100000) + Offset.Y;
		octaveOffsets.Add(FVector2D(offsetX, offsetY));
	}


	if (Scale <= 0)
	{
		Scale = 0.0001f;
	}

	TArray<float> TempArray;

	float maxNoiseHeight = TNumericLimits<float>::Min();
	float minNoiseHeight = TNumericLimits<float>::Max();

	float halfWidth = Width / 2;
	float halfHeight = Height / 2;

	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;

			for (int i = 0; i < Octaves; i++)
			{
				float sampleX = (x - halfWidth) / Scale * frequency + octaveOffsets[i].X;
				float sampleY = (y - halfHeight) / Scale * frequency + octaveOffsets[i].Y;

				float perlinValue = FMath::PerlinNoise2D(FVector2D(sampleX, sampleY));
				noiseHeight += perlinValue * amplitude;

				amplitude *= Persistance;
				frequency *= Lacunarity;
			}

			if (noiseHeight > maxNoiseHeight)
			{
				maxNoiseHeight = noiseHeight;
			}
			else if (noiseHeight < minNoiseHeight)
			{
				minNoiseHeight = noiseHeight;
			}
			NoiseMap[x][y] = noiseHeight;
		}
	}

	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			NoiseMap[x][y] = UKismetMathLibrary::NormalizeToRange(NoiseMap[x][y], minNoiseHeight, maxNoiseHeight);
		}
	}
	
	return NoiseMap;
}

TArray<TArray<int>> UGenerateMeshVariables_PN::NormalizeMap(TArray<TArray<float>> _floatMap)
{
	TArray<TArray<int>> NormalizedMap;
	TArray<int> temp;

	temp.Init(-1, Height);
	NormalizedMap.Init(temp, Width);

	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			if (_floatMap[x][y] <= ThresholdValueWall)
			{
				NormalizedMap[x][y] = 1;
			}
			else
			{
				NormalizedMap[x][y] = 0;
			}
		}
	}

	return NormalizedMap;
}

TArray<TArray<int>> UGenerateMeshVariables_PN::GenerateMapMatrix()
{
	TArray<TArray<float>> FloatMap = GenerateNoiseMap();
	Map = NormalizeMap(FloatMap);

	ProcessMap();

	Map = BorderMap(Map);

	return Map;
}

TArray<TArray<int>> UGenerateMeshVariables_PN::BorderMap(TArray<TArray<int>> _map)
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

void UGenerateMeshVariables_PN::ProcessMap()
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

void UGenerateMeshVariables_PN::ConnectClosestRooms(TArray<URoom*> allRooms, bool forceAccessibilityFromMainRoom)
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

void UGenerateMeshVariables_PN::CreatePassage(URoom* roomA, URoom* roomB, FVector2D tileA, FVector2D tileB)
{
	URoom::ConnectRooms(roomA, roomB);

	TArray<FVector2D> line = GetLine(tileA, tileB);

	for (FVector2D c : line)
	{
		DrawCircle(c, FMath::RandRange(1, 3));
	}
}

void UGenerateMeshVariables_PN::DrawCircle(FVector2D c, int r)
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

TArray<FVector2D> UGenerateMeshVariables_PN::GetLine(FVector2D from, FVector2D to)
{
	TArray<FVector2D> line;

	int x = from.X;
	int y = from.Y;

	int dx = to.X - from.X;
	int dy = to.Y - from.Y;

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
		line.Add(FVector2D(x,y));

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
}

TArray<FVector2D> UGenerateMeshVariables_PN::GetRegionTiles(int startX, int startY)
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
						queue.Enqueue(FVector2D(x,y));
					}
				}
			}
		}
	}

	return tiles;
}

TArray<TArray<FVector2D>> UGenerateMeshVariables_PN::GetRegions(int tileType)
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

bool UGenerateMeshVariables_PN::IsInMapRange(int x, int y)
{
	return x >= 0 && x < Width && y >= 0 && y < Height;
}
