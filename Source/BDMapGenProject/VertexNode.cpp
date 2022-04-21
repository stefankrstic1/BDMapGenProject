// Fill out your copyright notice in the Description page of Project Settings.


#include "VertexNode.h"

UVertexNode* UVertexNode::MAKE(FVector _position)
{
	UVertexNode* v = NewObject<UVertexNode>();
	v->init(_position);
	return v;
}

void UVertexNode::init(FVector _position)
{
	Position = _position;
}

void UVertexNode::SetVertexIndex(int _value)
{
	VertexIndex = _value;
}
