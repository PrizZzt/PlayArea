// Fill out your copyright notice in the Description page of Project Settings.

#include "TPlayerController.h"
#include "Engine.h"
#include "UserWidget.h"

ATPlayerController::ATPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	gameInstance = nullptr;
}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = (UTGameInstance*)GetWorld()->GetGameInstance();

	InputComponent->BindAction("ToggleTable", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowTable);
	InputComponent->BindAction("ToggleTable", EInputEvent::IE_Released, this, &ATPlayerController::HideTable);
}

void ATPlayerController::ShowTable()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "ShowTable");

	if (gameInstance->PointsTableInstance->GetIsVisible() == false)
	{
		gameInstance->PointsTableInstance->AddToViewport();
	}
}

void ATPlayerController::HideTable()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "HideTable");
	if (gameInstance->PointsTableInstance->GetIsVisible())
	{
		gameInstance->PointsTableInstance->RemoveFromViewport();
	}
}
