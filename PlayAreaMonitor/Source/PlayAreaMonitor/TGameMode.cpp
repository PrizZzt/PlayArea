// Fill out your copyright notice in the Description page of Project Settings.

#include "TGameMode.h"
#include "TPlayerController.h"

ATGameMode::ATGameMode()
{
	PlayerControllerClass = ATPlayerController::StaticClass();
}
