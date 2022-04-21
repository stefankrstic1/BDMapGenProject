// Fill out your copyright notice in the Description page of Project Settings.


#include "Direction2D.h"

UDirection2D* UDirection2D::MAKE()
{
	UDirection2D* d = NewObject<UDirection2D>();
	d->init();
	return d;
}

void UDirection2D::init()
{
	CardinalDirectionsList.Add(FVector2D(0, 1));
	CardinalDirectionsList.Add(FVector2D(1, 0));
	CardinalDirectionsList.Add(FVector2D(0, -1));
	CardinalDirectionsList.Add(FVector2D(-1, 0));

}

FVector2D UDirection2D::GetRandomCardinalDirection()
{
	Stream.GenerateNewSeed();
	return CardinalDirectionsList[Stream.FRandRange(0, CardinalDirectionsList.Num())];
}
