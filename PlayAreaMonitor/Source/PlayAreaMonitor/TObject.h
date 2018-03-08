// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TObject.generated.h"

UCLASS()
class PLAYAREAMONITOR_API ATObject : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Body;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int ID;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int Type;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int ToX;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int ToY;

	ATObject();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetTarget(int _X, int _Y);
};
