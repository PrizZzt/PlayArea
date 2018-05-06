// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDirector.generated.h"

UCLASS()
class PLAYAREAMONITOR_API ATDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDirector();
	void BeginPlay() override;
};
