// Fill out your copyright notice in the Description page of Project Settings.

#include "TObject.h"
#include "TGameInstance.h"

ATObject::ATObject()
{
	PrimaryActorTick.bCanEverTick = true;
	gameInstance = nullptr;

	to_delete = false;

	ID = 0;
	Type = 0;
}

void ATObject::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = (UTGameInstance*)GetWorld()->GetGameInstance();

	SetActorRotation(FRotator(0, FMath::RandHelper(4) * 90, 0));
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
			pos = target;
		}
		else
		{
			FVector dir = (target - pos).GetSafeNormal();
			FRotator rot = FMatrix(dir, FVector(-dir.Y, dir.X, dir.Z), FVector::UpVector, FVector::ZeroVector).Rotator();
			SetActorRotation(rot);

			if (distSquared <= 10000)
			{
				pos += dir * DeltaTime * 100;
			}
			else
			{
				pos += dir * DeltaTime * 200;
			}
		}
		SetActorLocation(pos);
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
		if (Type != 0)
			SetTypeInternal(_type, Type);
		Type = _type;
		bool is_resurrect_type;
		IsResurrectType(Type, is_resurrect_type);
		if (is_resurrect_type)
			SetActorLocation(FVector(ToX * 100, ToY * 100, 0));
	}
}
