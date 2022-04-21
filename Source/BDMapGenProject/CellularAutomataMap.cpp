// Fill out your copyright notice in the Description page of Project Settings.


#include "CellularAutomataMap.h"

// Sets default values
ACellularAutomataMap::ACellularAutomataMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FMapMeshVariables ACellularAutomataMap::GenerateMap()
{
	MapGeneration = UGenerateMeshVariables_CA::MAKE(Width, Height, RandomFillPercent, UseRandomStream, Stream, CountOfMapSmoothing, BorderSize, SquareSize);

	return MapGeneration->GenerateMap();
}

// Called when the game starts or when spawned
void ACellularAutomataMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACellularAutomataMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

