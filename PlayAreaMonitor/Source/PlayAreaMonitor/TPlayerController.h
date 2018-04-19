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
	bool ManualControl;

	ATPlayerController();

	void BeginPlay()override;

	void ShowTable();
	void HideTable();
	void StartLooking();
	void StopLooking();

	void MouseLookX(float _scale);
	void MouseLookY(float _scale);
	void MoveForward(float _scale);
	void MoveRight(float _scale);
};
