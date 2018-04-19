// Fill out your copyright notice in the Description page of Project Settings.

#include "TPlayerController.h"
#include "Engine.h"
#include "UserWidget.h"
#include "TSpectator.h"

ATPlayerController::ATPlayerController()
{
	gameInstance = nullptr;
}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();
	StopLooking();
	gameInstance = (UTGameInstance*)GetWorld()->GetGameInstance();

	InputComponent->BindAction("ToggleTable", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowTable);
	InputComponent->BindAction("ToggleTable", EInputEvent::IE_Released, this, &ATPlayerController::HideTable);
	InputComponent->BindAction("ToggleMouseLook", EInputEvent::IE_Pressed, this, &ATPlayerController::StartLooking);
	InputComponent->BindAction("ToggleMouseLook", EInputEvent::IE_Released, this, &ATPlayerController::StopLooking);
	InputComponent->BindAxis("MouseLookX", this, &ATPlayerController::MouseLookX);
	InputComponent->BindAxis("MouseLookY", this, &ATPlayerController::MouseLookY);
	InputComponent->BindAxis("MoveForward", this, &ATPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATPlayerController::MoveRight);
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

void ATPlayerController::StartLooking()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	ManualControl = true;
}

void ATPlayerController::StopLooking()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	ManualControl = false;
}

void ATPlayerController::MouseLookX(float _scale)
{
	if (ManualControl && _scale != 0.0f)
	{
		AddYawInput(100.f * _scale * GetWorld()->GetDeltaSeconds());
	}
}

void ATPlayerController::MouseLookY(float _scale)
{
	if (ManualControl && _scale != 0.0f)
	{
		AddPitchInput(100.f * -_scale * GetWorld()->GetDeltaSeconds());
	}
}

void ATPlayerController::MoveForward(float _scale)
{
	if (ManualControl && _scale != 0.0f)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), _scale);
	}
}

void ATPlayerController::MoveRight(float _scale)
{
	if (ManualControl && _scale != 0.0f)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), _scale);
	}
}
