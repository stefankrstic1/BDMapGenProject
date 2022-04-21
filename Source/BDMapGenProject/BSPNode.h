// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BSPNode.generated.h"

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UBSPNode : public UObject
{
	GENERATED_BODY()
	
public:
	FVector2D Point;
	int Width;
	int Height;
	UBSPNode* First = nullptr;
	UBSPNode* Second = nullptr;

	static UBSPNode* MAKE(FVector2D _point, int _width, int _height);
	void init(FVector2D _point, int _width, int _height);

	bool Split(FRandomStream _stream, int _smallestRegion);

};
