// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerateMeshVariables_CA.h"
#include "MapMeshVariables.h"
#include "CellularAutomataMap.generated.h"

class UGenerateMeshVariables_CA;

UCLASS()
class BDMAPGENPROJECT_API ACellularAutomataMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACellularAutomataMap();

	UGenerateMeshVariables_CA* MapGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int Width = 80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int Height = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 100))
	int RandomFillPercent = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	bool UseRandomStream = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	FRandomStream Stream;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int CountOfMapSmoothing = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int BorderSize = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int SquareSize = 2;


	UFUNCTION(BlueprintCallable)
	FMapMeshVariables GenerateMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
