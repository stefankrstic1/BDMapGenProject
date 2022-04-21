// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ControlVertexNode.h"
#include "Square.generated.h"

/**
 * 
 */
UCLASS()
class BDMAPGENPROJECT_API USquare : public UObject
{
	GENERATED_BODY()
	
public:
	UControlVertexNode* TopLeft;
	UControlVertexNode* TopRight;
	UControlVertexNode* BottomRight;
	UControlVertexNode* BottomLeft;

	UVertexNode* CentreTop;
	UVertexNode* CentreRight;
	UVertexNode* CentreBottom;
	UVertexNode* CentreLeft;

	int Configuration = 0;

	static USquare* MAKE(UControlVertexNode* _topLeft, UControlVertexNode* _topRight, UControlVertexNode* _bottomRight, UControlVertexNode* _bottomLeft);
	void init(UControlVertexNode* _topLeft, UControlVertexNode* _topRight, UControlVertexNode* _bottomRight, UControlVertexNode* _bottomLeft);
};
