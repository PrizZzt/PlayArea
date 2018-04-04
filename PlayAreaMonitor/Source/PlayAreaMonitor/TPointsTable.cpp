// Fill out your copyright notice in the Description page of Project Settings.

#include "TPointsTable.h"
#include "TPointsRow.h"
#include "TGameInstance.h"

void UTPointsTable::StartUpdate()
{
	if (List)
	{
		int32 rowCount = List->GetChildrenCount();
		for (int32 i = 0; i < rowCount; i++)
		{
			Cast<UTPointsRow>(List->GetChildAt(i))->ToDelete = true;
		}
	}
}

void UTPointsTable::SetPlayerName(uint8 _id, const FString &_name)
{
	if (List)
	{
		UTPointsRow *row;
		int32 rowCount = List->GetChildrenCount();
		for (int32 i = 0; i < rowCount; i++)
		{
			row = Cast<UTPointsRow>(List->GetChildAt(i));
			if (row && row->ID == _id)
			{
				row->Name = _name;
				row->ToDelete = false;
				return;
			}
		}
		row = CreateWidget<UTPointsRow>(gameInstance, gameInstance->PointsRow);
		row->SetPlayerName(_id, _name);
		List->AddChild(row);
	}
}

void UTPointsTable::SetPlayerPoints(uint8 _id, int32 _points)
{
	if (List)
	{
		UTPointsRow *row;
		int32 rowCount = List->GetChildrenCount();
		for (int32 i = 0; i < rowCount; i++)
		{
			row = Cast<UTPointsRow>(List->GetChildAt(i));
			if (row && row->ID == _id)
			{
				row->Points = _points;
				row->ToDelete = false;
				return;
			}
		}
		row = CreateWidget<UTPointsRow>(gameInstance, gameInstance->PointsRow);
		row->SetPlayerPoints(_id, _points);
		List->AddChild(row);
	}
}

void UTPointsTable::EndUpdate()
{
	if (List)
	{
		int32 rowCount = List->GetChildrenCount();
		for (int32 i = 0; i < rowCount; i++)
		{
			UTPointsRow *row = Cast<UTPointsRow>(List->GetChildAt(i));
			if (row && row->ToDelete == true)
			{
				row->RemoveFromParent();
			}
		}
	}
}
