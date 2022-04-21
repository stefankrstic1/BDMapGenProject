// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

URoom* URoom::MAKE(TArray<FVector2D> _roomTiles, TArray<TArray<int>> _map)
{
	URoom* r = NewObject<URoom>();
	r->init(_roomTiles, _map);
	return r;
}

void URoom::init(TArray<FVector2D> _roomTiles, TArray<TArray<int>> _map)
{
	if (_roomTiles.Num() == 0)
		return;

	tiles = _roomTiles;
	roomSize = _roomTiles.Num();

	for (FVector2D tile : tiles)
	{
		for (int x = tile.X - 1; x < tile.X + 1; x++)
		{
			for (int y = tile.Y - 1; y < tile.Y + 1; y++)
			{
				if (x == tile.X || y == tile.Y)
				{
					if (x != -1 && y != -1) {
						if (_map[x][y] == 1)
						{
							edgeTiles.Add(tile);
						}
					}
				}
			}
		}
	}
}

void URoom::ConnectRooms(URoom* roomA, URoom* roomB)
{
	if (roomA->bIsAccessibleFromMainRoom)
	{
		roomB->SetAccessibleFromMainRoom();
	}
	else if (roomB->bIsAccessibleFromMainRoom)
	{
		roomA->SetAccessibleFromMainRoom();
	}

	roomA->connectedRooms.Add(roomB);
	roomB->connectedRooms.Add(roomA);
}

bool URoom::IsConnected(URoom* otherRoom)
{
	return connectedRooms.Contains(otherRoom);
}

void URoom::SetAccessibleFromMainRoom()
{
	if (!bIsAccessibleFromMainRoom)
	{
		bIsAccessibleFromMainRoom = true;
		for (URoom* room : connectedRooms)
		{
			room->SetAccessibleFromMainRoom();
		}
	}
}
