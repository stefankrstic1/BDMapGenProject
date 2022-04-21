// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Direction2D.generated.h"



/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UDirection2D : public UObject
{
	GENERATED_BODY()

public:
	FRandomStream Stream = FRandomStream();
	TArray<FVector2D> CardinalDirectionsList;

	static UDirection2D* MAKE();
	void init();

	FVector2D GetRandomCardinalDirection();
};