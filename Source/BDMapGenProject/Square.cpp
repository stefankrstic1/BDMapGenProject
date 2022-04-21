// Fill out your copyright notice in the Description page of Project Settings.


#include "Square.h"

USquare* USquare::MAKE(UControlVertexNode* _topLeft, UControlVertexNode* _topRight, UControlVertexNode* _bottomRight, UControlVertexNode* _bottomLeft)
{
	USquare* s = NewObject<USquare>();
	s->init(_topLeft, _topRight, _bottomRight, _bottomLeft);
	return s;
}

void USquare::init(UControlVertexNode* _topLeft, UControlVertexNode* _topRight, UControlVertexNode* _bottomRight, UControlVertexNode* _bottomLeft)
{
	TopLeft = _topLeft;
	TopRight = _topRight;
	BottomRight = _bottomRight;
	BottomLeft = _bottomLeft;

	CentreTop = TopLeft->Right;
	CentreRight = BottomRight->Above;
	CentreBottom = BottomLeft->Right;
	CentreLeft = BottomLeft->Above;

	if (TopLeft->Active)
		Configuration += 8;
	if (TopRight->Active)
		Configuration += 4;
	if (BottomRight->Active)
		Configuration += 2;
	if (BottomLeft->Active)
		Configuration += 1;
}
