// Fill out your copyright notice in the Description page of Project Settings.


#include "BSPNode.h"

UBSPNode* UBSPNode::MAKE(FVector2D _point, int _width, int _height)
{
	UBSPNode* n = NewObject<UBSPNode>();
	n->init(_point, _width, _height);
	return n;
}

void UBSPNode::init(FVector2D _point, int _width, int _height)
{
	Point = _point;
	Width = _width;
	Height = _height;
}

bool UBSPNode::Split(FRandomStream _stream, int _smallestRegion)
{
	_stream.GenerateNewSeed();
	if (Width <= _smallestRegion || Height <= _smallestRegion)
	{
		return false;
	}

	bool splitHorizontal = _stream.RandRange(0,100) < 50;
	if (Width > Height && (Width *1.0f) / (Height * 1.0f) >= 1.2)
	{
		splitHorizontal = false;
	}
	else if (Height > Width && (Height * 1.0f) / (Width * 1.0f) >= 1.2)
	{
		splitHorizontal = true;
	}

	int maxRegion = (splitHorizontal ? Height : Width) - _smallestRegion;
	//UE_LOG(LogTemp, Error, TEXT("SmallestRegion: %d, MaxRegion: %d"), _smallestRegion, maxRegion);

	if (maxRegion <= _smallestRegion)
	{
		return false;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Main X: %f, Y: %f, Width: %d, Height: %d"), Point.X, Point.Y, Width, Height);
	if(splitHorizontal)
	{
		int Separator = _stream.RandRange(_smallestRegion, maxRegion);
		//UE_LOG(LogTemp, Warning, TEXT("Horizontal Split"));
		//UE_LOG(LogTemp, Warning, TEXT("Separator: %d"), Separator);

		First = UBSPNode::MAKE(FVector2D(Point.X, Point.Y), Width, Separator);
		//UE_LOG(LogTemp, Warning, TEXT("First X: %f, Y: %f, Width: %d, Height: %d"), First->Point.X, First->Point.Y, Width, Separator);

		Second = UBSPNode::MAKE(FVector2D(Point.X, Point.Y + Separator), Width,Height - Separator);
		//UE_LOG(LogTemp, Warning, TEXT("Second X: %f, Y: %f, Width: %d, Height: %d"), Second->Point.X, Second->Point.Y, Width, Height - Separator);

	}
	else 
	{
		int Separator = _stream.RandRange(_smallestRegion, maxRegion);
		//UE_LOG(LogTemp, Warning, TEXT("Vertical Split"));
		//UE_LOG(LogTemp, Warning, TEXT("Separator: %d"), Separator);

		First = UBSPNode::MAKE(FVector2D(Point.X, Point.Y), Separator, Height);
		//UE_LOG(LogTemp, Warning, TEXT("First X: %f, Y: %f, Width: %d, Height: %d"), First->Point.X, First->Point.Y, Separator, Height);


		Second = UBSPNode::MAKE(FVector2D(Point.X + Separator, Point.Y), Width - Separator, Height);
		//UE_LOG(LogTemp, Warning, TEXT("Second X: %f, Y: %f, Width: %d, Height: %d"), Second->Point.X, Second->Point.Y, Width - Separator, Height);
	}

	return true;
}
