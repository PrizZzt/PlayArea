// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UTGameInstance;

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TObject.generated.h"

UCLASS()
class PLAYAREAMONITOR_API ATObject : public AActor
{
	GENERATED_BODY()
		UTGameInstance *gameInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent *Body;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	bool to_delete;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 ID;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 Type;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 BaseX;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 BaseY;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 ToX;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 ToY;

	ATObject();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetTarget(int _X, int _Y);
	UFUNCTION(BlueprintCallable)
		void SetType(int _type);

	static uint8_t GetGroup(uint8_t _type)
	{
		switch (_type)
		{
		case 1://UNDESTRUCTIBLE_WALL
		case 13://BOOM
		case 14://BOMB_1
		case 15://BOMB_2
		case 16://BOMB_3
		case 17://BOMB_4
		case 18://BOMB_5
			return 0;

		case 2://DESTRUCTIBLE_WALL
		case 3://DESTROYED_WALL
			return 1;

		case 4://PLAYER
		case 5://PLAYER_WITH_BOMB_1
		case 6://PLAYER_WITH_BOMB_2
		case 7://PLAYER_WITH_BOMB_3
		case 8://PLAYER_WITH_BOMB_4
		case 9://PLAYER_WITH_BOMB_5
		case 10://DEAD_PLAYER
			return 2;

		case 11://MEATCHOPPER
		case 12://DEAD_MEATCHOPPER
			return 3;

		default:
			return 0;
		}
	}
};
