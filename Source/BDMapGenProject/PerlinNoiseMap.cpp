// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoiseMap.h"

// Sets default values
APerlinNoiseMap::APerlinNoiseMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FMapMeshVariables APerlinNoiseMap::GenerateMap()
{
	MapGeneration = UGenerateMeshVariables_PN::MAKE(Width, Height, UseRandomStream, Stream, BorderSize, SquareSize, ThresholdValueWall, Scale, Octaves, Persistance, Lacinarity, Offset);

	return MapGeneration->GenerateMap();
}

// Called when the game starts or when spawned
void APerlinNoiseMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APerlinNoiseMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

