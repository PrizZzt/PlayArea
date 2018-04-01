// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPointsRow.generated.h"

/**
 * 
 */
UCLASS()
class PLAYAREAMONITOR_API UTPointsRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 ID;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		FString Name;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int32 Points;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool ToDelete;

	UFUNCTION(BlueprintCallable)
		void SetPlayerName(uint8 _id, const FString &_name);
	UFUNCTION(BlueprintCallable)
		void SetPlayerPoints(uint8 _id, int32 _points);
};
