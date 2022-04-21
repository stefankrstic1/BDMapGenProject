// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerateMeshVariables_BSP.h"
#include "BSPMap.generated.h"

class UGenerateMeshVariables_BSP;

UCLASS()
class BDMAPGENPROJECT_API ABSPMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABSPMap();

	UGenerateMeshVariables_BSP* MapGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int Width = 80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int Height = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	bool UseRandomStream = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	FRandomStream Stream;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int BorderSize = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int SquareSize = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int NumberOfRooms = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int SmallestRegion = 20;

	UFUNCTION(BlueprintCallable)
	FMapMeshVariables GenerateMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
