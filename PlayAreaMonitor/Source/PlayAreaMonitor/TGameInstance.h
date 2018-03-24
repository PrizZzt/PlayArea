// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <thread>

#include "CoreMinimal.h"
#include "Networking.h"
#include "Engine/GameInstance.h"
#include "TObject.h"
#include "TGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PLAYAREAMONITOR_API UTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FSocket *Socket;
	TArray<ATObject*> objects;
	uint8_t size_x, size_y;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Objects")
		TSubclassOf<ATObject> UndestructibleWall;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Objects")
		TSubclassOf<ATObject> DestructibleWall;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Objects")
		TSubclassOf<ATObject> Player;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Objects")
		TSubclassOf<ATObject> Meatchopper;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Objects")
		TSubclassOf<ATObject> Bomb;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Objects")
		TSubclassOf<ATObject> Boom;

	void Shutdown() override;

	UFUNCTION(BlueprintCallable)
		void Start(FString _address, int32 _port);
	UFUNCTION(BlueprintCallable)
		void Stop();

	void SetSize(uint8_t _size_x, uint8_t _size_y)
	{
		size_x = _size_x;
		size_y = _size_y;
	}

	void UpdateObject(uint8_t _id, uint8_t _type, uint8_t _x, uint8_t _y);
};
