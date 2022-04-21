// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerateMeshVariables_RW.h"
#include "RandomWalkMap.generated.h"

class UGenerateMeshVariables_RW;

UCLASS()
class BDMAPGENPROJECT_API ARandomWalkMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomWalkMap();

	UGenerateMeshVariables_RW* MapGeneration;

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
	int Iterations = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	int WalkLength = 10;

	UFUNCTION(BlueprintCallable)
	FMapMeshVariables GenerateMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
