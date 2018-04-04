// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UTGameInstance;

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBox.h"
#include "TPointsTable.generated.h"

/**
 * 
 */
UCLASS()
class PLAYAREAMONITOR_API UTPointsTable : public UUserWidget
{
	GENERATED_BODY()

public:
	UTGameInstance *gameInstance;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UScrollBox *List;
	UFUNCTION(BlueprintImplementableEvent)
		void InitList_BP();

	void StartUpdate();
	void SetPlayerName(uint8 _id, const FString &_name);
	void SetPlayerPoints(uint8 _id, int32 _points);
	void EndUpdate();
};
