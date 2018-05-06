// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TObject.h"
#include "DestructibleComponent.h"
#include "TDestructibleObject.generated.h"

/**
 * 
 */
UCLASS()
class PLAYAREAMONITOR_API ATDestructibleObject : public ATObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent *Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UDestructibleComponent *Body;

	UDestructibleMesh *mesh;

	ATDestructibleObject();

	void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Destroy();
	UFUNCTION(BlueprintCallable)
		void Renew();

	void SetRotation(FRotator &_rotation)override;
};
