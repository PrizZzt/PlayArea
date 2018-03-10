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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meshes")
		TAssetPtr<UStaticMesh> UndestructibleWallMesh;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meshes")
		TAssetPtr<UStaticMesh> DestructibleWallMesh;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meshes")
		TAssetPtr<UStaticMesh> PlayerMesh;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meshes")
		TAssetPtr<UStaticMesh> MeatchopperMesh;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meshes")
		TAssetPtr<UStaticMesh> BombMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Types")
		TSubclassOf<ATObject> ObjectType;

	FSocket *Socket;

	UFUNCTION(BlueprintCallable)
		void Start(FString _address, int32 _port);
	UFUNCTION(BlueprintCallable)
		void Stop();
};
