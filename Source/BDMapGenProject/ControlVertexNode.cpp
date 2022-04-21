// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlVertexNode.h"

UControlVertexNode* UControlVertexNode::MAKE(FVector _position, bool _active, float _squareSize)
{
	UControlVertexNode* c = NewObject<UControlVertexNode>();
	c->init(_position, _active, _squareSize);
	return c;
}

void UControlVertexNode::init(FVector _position, bool _active, float _squareSize)
{
	Position = _position;
	Active = _active;
	Above = UVertexNode::MAKE(_position + FVector(1, 0, 0) * _squareSize / 2.f);
	Right = UVertexNode::MAKE(_position + FVector(0, 1, 0) * _squareSize / 2.f);
}
