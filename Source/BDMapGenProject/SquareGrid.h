// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ControlVertexNode.h"
#include "Square.h"
#include "SquareGrid.generated.h"

class UControlVertexNode;
class USquare;

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API USquareGrid : public UObject
{
	GENERATED_BODY()

public:
	TArray<TArray<USquare*>> Squares;

	static USquareGrid* MAKE();	
	TArray<TArray<USquare*>> GenerateSquareGrid(TArray<TArray<int>> _map, float _squareSize);
};
