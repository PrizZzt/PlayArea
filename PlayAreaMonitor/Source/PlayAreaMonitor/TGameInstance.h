// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <thread>

#include "CoreMinimal.h"
#include "Networking.h"
#include "Engine/GameInstance.h"
#include "TObject.h"
#include "TPointsRow.h"
#include "TPointsTable.h"
#include "TGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PLAYAREAMONITOR_API UTGameInstance : public UGameInstance
{
	GENERATED_BODY()

		bool check_names;

public:
	FSocket *Socket;
	TArray<ATObject*> objects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Size")
		uint8 SizeX;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Size")
		uint8 SizeY;

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
		TSubclassOf<UTPointsRow> PointsRow;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
		TSubclassOf<UTPointsTable> PointsTable;

	UPROPERTY(/*BlueprintReadOnly, EditAnywhere, Category = "UI"*/)
		UTPointsTable *PointsTableInstance;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ground")
		AActor *Ground;

	void Init()override;
	void Shutdown()override;

	UFUNCTION(BlueprintCallable)
		void Start(FString _address, int32 _port);
	UFUNCTION(BlueprintCallable)
		void Stop();

	void SetSize(uint8_t _size_x, uint8_t _size_y)
	{
		if (SizeX != _size_x || SizeY != _size_y)
		{
			SizeX = _size_x;
			SizeY = _size_y;

			SetSize_BP();
		}
	}
	UFUNCTION(BlueprintImplementableEvent)
		void SetSize_BP();

	void UpdateObject(uint8_t _id, uint8_t _type, uint8_t _x, uint8_t _y);
	void UpdatePlayerName(uint8_t _id, FString &_name);
	void CheckAdditionalInfo();

	int GetNextPlayerIndex(int startIndex);
};
