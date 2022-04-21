// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Room.generated.h"

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API URoom : public UObject
{
	GENERATED_BODY()
	
public:
	TArray<FVector2D> tiles;
	TArray<FVector2D> edgeTiles;
	TArray<URoom*> connectedRooms;
	int roomSize;
	bool bIsAccessibleFromMainRoom;
	bool bIsMainRoom;

	static URoom* MAKE(TArray<FVector2D> _roomTiles, TArray<TArray<int>> _map);
	void init(TArray<FVector2D> _roomTiles, TArray<TArray<int>> _map);

	static void ConnectRooms(URoom* roomA, URoom* roomB);

	bool IsConnected(URoom* otherRoom);

	void SetAccessibleFromMainRoom();
};
