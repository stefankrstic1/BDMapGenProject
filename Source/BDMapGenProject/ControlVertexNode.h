// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VertexNode.h"
#include "ControlVertexNode.generated.h"

class UVertexNode;
 
/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API UControlVertexNode : public UVertexNode
{
	GENERATED_BODY()
	
public:
	bool Active;
	UVertexNode* Above;
	UVertexNode* Right;

	static UControlVertexNode* MAKE(FVector _position, bool _active, float _squareSize);
	void init(FVector _position, bool _active, float _squareSize);
};
