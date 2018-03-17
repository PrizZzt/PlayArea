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

private:
	std::thread receive_thread;
	bool is_running;
	void receive_loop();

	TArray<ATObject*> objects;

	uint8_t size_x, size_y;

public:
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

	FSocket *Socket;

	void Shutdown() override;

	UFUNCTION(BlueprintCallable)
		void Start(FString _address, int32 _port);
	UFUNCTION(BlueprintCallable)
		void Stop();
};
