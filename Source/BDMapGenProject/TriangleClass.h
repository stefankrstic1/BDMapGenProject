// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TriangleClass.generated.h"

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UTriangleClass : public UObject
{
	GENERATED_BODY()
	
public:
	int vertexIndexA;
	int vertexIndexB;
	int vertexIndexC;

	TArray<int> vertices;

	static UTriangleClass* MAKE(int _a, int _b, int _c);
	void init(int _a, int _b, int _c);

	bool Contains(int vertexIndex);
};
