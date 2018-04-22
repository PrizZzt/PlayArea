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

	InputComponent->BindAction("SetStaticLook", EInputEvent::IE_Released, this, &ATPlayerController::SetStaticLook);
	InputComponent->BindAction("SetManualLook", EInputEvent::IE_Released, this, &ATPlayerController::SetManualLook);
	InputComponent->BindAction("SetPlayerLook", EInputEvent::IE_Released, this, &ATPlayerController::SetPlayerLook);

	SetStaticLook();
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
		switch (LookType)
		{
		case ELookType::LT_STATIC:
			AddYawInput(100.f * _scale * GetWorld()->GetDeltaSeconds());
			break;

		case ELookType::LT_MANUAL:
			AddYawInput(100.f * _scale * GetWorld()->GetDeltaSeconds());
			break;
		}
		FRotator r = GetPawn()->GetActorRotation();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Pitch=" + FString::FromInt(r.Pitch) + "Yaw=" + FString::FromInt(r.Yaw) + "Roll=" + FString::FromInt(r.Roll));
	}
}

void ATPlayerController::MouseLookY(float _scale)
{
	if (ManualControl && _scale != 0.0f && LookType == ELookType::LT_MANUAL)
	{
		AddPitchInput(100.f * -_scale * GetWorld()->GetDeltaSeconds());
		FRotator r = GetPawn()->GetActorRotation();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Pitch=" + FString::FromInt(r.Pitch) + "Yaw=" + FString::FromInt(r.Yaw) + "Roll=" + FString::FromInt(r.Roll));
	}
}

void ATPlayerController::MoveForward(float _scale)
{
	if (ManualControl && _scale != 0.0f && LookType == ELookType::LT_MANUAL)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), _scale);
	}
}

void ATPlayerController::MoveRight(float _scale)
{
	if (ManualControl && _scale != 0.0f && LookType == ELookType::LT_MANUAL)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), _scale);
	}
}

void ATPlayerController::SetStaticLook()
{
	LookType = ELookType::LT_STATIC;
	GetPawn()->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));

	float size = gameInstance->SizeX > gameInstance->SizeY
		? gameInstance->SizeX * 100
		: gameInstance->SizeY * 100;

	GetPawn()->SetActorLocation(FVector(size / 2 - 50, size / 2 - 50, size + 200));
	SetControlRotation(FRotator(-90.0f, 0.0f, 0.0f));
}

void ATPlayerController::SetManualLook()
{
	LookType = ELookType::LT_MANUAL;
	GetPawn()->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
}

void ATPlayerController::SetPlayerLook()
{
	AActor *playerToAttach;
	if (LookType == ELookType::LT_PLAYER)
	{
		indexOfPlayerToAttach = gameInstance->GetNextPlayerIndex(indexOfPlayerToAttach);
	}
	else
	{
		LookType = ELookType::LT_PLAYER;
		if (ATObject::GetGroup(gameInstance->objects[indexOfPlayerToAttach]->Type) != ATObject::PLAYER_GROUP)
			indexOfPlayerToAttach = gameInstance->GetNextPlayerIndex(indexOfPlayerToAttach);
	}
	playerToAttach = gameInstance->objects[indexOfPlayerToAttach];
	GetPawn()->AttachToActor(playerToAttach, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
}
