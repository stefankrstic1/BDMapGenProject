// Fill out your copyright notice in the Description page of Project Settings.


#include "BSPMap.h"

// Sets default values
ABSPMap::ABSPMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FMapMeshVariables ABSPMap::GenerateMap()
{
	MapGeneration = UGenerateMeshVariables_BSP::MAKE(Width, Height, UseRandomStream, Stream, BorderSize, SquareSize, NumberOfRooms, SmallestRegion);

	return MapGeneration->GenerateMap();
}

// Called when the game starts or when spawned
void ABSPMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABSPMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

