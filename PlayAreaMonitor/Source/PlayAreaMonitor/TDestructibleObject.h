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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent *Body;

public:
	UDestructibleMesh *mesh;

	ATDestructibleObject();

	void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Destroy();
	UFUNCTION(BlueprintCallable)
		void Renew();
};
