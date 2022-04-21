// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerateMeshVariables_PN.h"
#include "MapMeshVariables.h"
#include "PerlinNoiseMap.generated.h"

class UGenerateMeshVariables_PN;

UCLASS()
class BDMAPGENPROJECT_API APerlinNoiseMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerlinNoiseMap();

	UGenerateMeshVariables_PN* MapGeneration;

	// Dodaj za UseRandomStream da bude korisno
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int Width = 80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int Height = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	bool UseRandomStream = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	FRandomStream Stream = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int BorderSize = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int SquareSize = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	float ThresholdValueWall = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	float Scale = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int Octaves = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 1))
	float Persistance = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	float Lacinarity = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	FVector2D Offset = FVector2D(0,0);


	UFUNCTION(BlueprintCallable)
	FMapMeshVariables GenerateMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
