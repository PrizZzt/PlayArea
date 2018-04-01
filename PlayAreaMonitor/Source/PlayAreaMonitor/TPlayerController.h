// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TGameInstance.h"
#include "TPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PLAYAREAMONITOR_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()
		UTGameInstance *gameInstance;

public:
	UUserWidget *PointsTable;
	ATPlayerController();

	void BeginPlay()override;

	void ShowTable();
	void HideTable();
};
