// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VertexNode.generated.h"

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UVertexNode : public UObject
{
	GENERATED_BODY()

public:
	FVector Position;
	int VertexIndex = -1;

	static UVertexNode* MAKE(FVector _position);
	void init(FVector _position);

	void SetVertexIndex(int _value);
};
