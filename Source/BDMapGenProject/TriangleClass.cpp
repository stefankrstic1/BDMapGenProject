// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleClass.h"

UTriangleClass* UTriangleClass::MAKE(int _a, int _b, int _c)
{
	UTriangleClass* t = NewObject<UTriangleClass>();
	t->init(_a, _b, _c);
	return t;
}

void UTriangleClass::init(int _a, int _b, int _c)
{
	vertexIndexA = _a;
	vertexIndexB = _b;
	vertexIndexC = _c;

	vertices.Add(vertexIndexA);
	vertices.Add(vertexIndexB);
	vertices.Add(vertexIndexC);
}

bool UTriangleClass::Contains(int vertexIndex)
{
	return vertexIndex == vertexIndexA || vertexIndex == vertexIndexB || vertexIndex == vertexIndexC;
}
