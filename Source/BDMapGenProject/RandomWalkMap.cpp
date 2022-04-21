// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomWalkMap.h"

// Sets default values
ARandomWalkMap::ARandomWalkMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FMapMeshVariables ARandomWalkMap::GenerateMap()
{
	MapGeneration = UGenerateMeshVariables_RW::MAKE(Width, Height, UseRandomStream, Stream, BorderSize, SquareSize, Iterations, WalkLength);
	return MapGeneration->GenerateMap();
}

// Called when the game starts or when spawned
void ARandomWalkMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARandomWalkMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

