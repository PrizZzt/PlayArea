// Fill out your copyright notice in the Description page of Project Settings.

#include "TPointsRow.h"

void UTPointsRow::SetPlayerName(uint8 _id, const FString & _name)
{
	ID = _id;
	Name = _name;
	ToDelete = false;
}

void UTPointsRow::SetPlayerPoints(uint8 _id, int32 _points)
{
	ID = _id;
	Points = _points;
	ToDelete = false;
}
