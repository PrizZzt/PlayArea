// Fill out your copyright notice in the Description page of Project Settings.

#include "TObject.h"
#include "TGameInstance.h"

ATObject::ATObject()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));

	PrimaryActorTick.bCanEverTick = true;
	gameInstance = nullptr;

	ID = 0;
	Type = 0;
}

void ATObject::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = (UTGameInstance*)GetWorld()->GetGameInstance();
}

void ATObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector target(ToX * 100, ToY * 100, 0);

	FVector pos = GetActorLocation();

	float speed = 100 * DeltaTime;
	float distSquared = FVector::DistSquared(pos, target);
	if (distSquared > 0)
	{
		if (distSquared < 1)
		{
			SetActorLocation(target);
		}
		else if (distSquared <= 10000)
		{
			pos += (target - pos).GetSafeNormal() * DeltaTime * 100;
			SetActorLocation(pos);
		}
		else
		{
			pos += (target - pos).GetSafeNormal() * DeltaTime * 200;
			SetActorLocation(pos);
		}
	}
}

void ATObject::SetTarget(int _X, int _Y)
{
	ToX = _X;
	ToY = _Y;
}

void ATObject::SetType(int _type)
{
	if (Type != _type && gameInstance)
	{
		UStaticMesh *new_mesh;
		UMaterialInterface *new_material;
		switch (_type)
		{
		case 1://UNDESTRUCTIBLE_WALL
			new_mesh = gameInstance->UndestructibleWallMesh.LoadSynchronous();
			new_material = gameInstance->UndestructibleWallMaterial.LoadSynchronous();
			break;

		case 2://DESTRUCTIBLE_WALL
		case 3://DESTROYED_WALL
			new_mesh = gameInstance->DestructibleWallMesh.LoadSynchronous();
			new_material = gameInstance->DestructibleWallMaterial.LoadSynchronous();
			break;

		case 4://PLAYER
		case 5://PLAYER_WITH_BOMB_1
		case 6://PLAYER_WITH_BOMB_2
		case 7://PLAYER_WITH_BOMB_3
		case 8://PLAYER_WITH_BOMB_4
		case 9://PLAYER_WITH_BOMB_5
		case 10://DEAD_PLAYER
			new_mesh = gameInstance->PlayerMesh.LoadSynchronous();
			new_material = gameInstance->PlayerMaterial.LoadSynchronous();
			break;

		case 11://MEATCHOPPER
		case 12://DEAD_MEATCHOPPER
			new_mesh = gameInstance->MeatchopperMesh.LoadSynchronous();
			new_material = gameInstance->MeatchopperMaterial.LoadSynchronous();
			break;

		case 13://BOOM
			new_mesh = nullptr;
			new_material = nullptr;
			break;

		case 14://BOMB_1
		case 15://BOMB_2
		case 16://BOMB_3
		case 17://BOMB_4
		case 18://BOMB_5
			new_mesh = gameInstance->UndestructibleWallMesh.LoadSynchronous();
			new_material = gameInstance->BombMaterial.LoadSynchronous();
			break;

		default:
			return;
		}
		Body->SetStaticMesh(new_mesh);
		Body->SetMaterial(0, new_material);
		Type = _type;
	}
}
