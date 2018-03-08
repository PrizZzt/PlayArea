// Fill out your copyright notice in the Description page of Project Settings.

#include "TObject.h"

ATObject::ATObject()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));

	PrimaryActorTick.bCanEverTick = true;
}

void ATObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector target(ToX, 0, ToY);

	FVector pos = GetActorLocation();

	float speed = 100 * DeltaTime;
	float distSquared = FVector::DistSquared(pos, target);

	if (distSquared > 0)
	{
		if (distSquared < DeltaTime * DeltaTime * 100)
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
