// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPointsTable.generated.h"

/**
 * 
 */
UCLASS()
class PLAYAREAMONITOR_API UTPointsTable : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void StartUpdate_BP();
	UFUNCTION(BlueprintImplementableEvent)
		void SetPlayerName_BP(uint8 _id, const FString &_name);
	UFUNCTION(BlueprintImplementableEvent)
		void SetPlayerPoints_BP(uint8 _id, int32 _points);
	UFUNCTION(BlueprintImplementableEvent)
		void EndUpdate_BP();
};
