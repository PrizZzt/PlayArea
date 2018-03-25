#pragma once

#include "FPlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		uint8 ID;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString Name;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 Points;

	FPlayerInfo()
	{
	}

	FPlayerInfo(uint8 _id, FString &_name)
	{
		Name = _name;
	}

	FPlayerInfo(uint8 _id, int32 _points)
	{
		Points = _points;
	}
};
