// Fill out your copyright notice in the Description page of Project Settings.

#include "TDestructibleObject.h"

ATDestructibleObject::ATDestructibleObject()
{
	Body = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Body"));

	RootComponent = Body;
	PrimaryActorTick.bCanEverTick = true;
}

void ATDestructibleObject::BeginPlay()
{
	Super::BeginPlay();
	mesh = Body->GetDestructibleMesh();
}

void ATDestructibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDestructibleObject::Destroy()
{
	Body->ApplyDamage(100, GetActorLocation(), GetActorLocation(), 500);
}

void ATDestructibleObject::Renew()
{
	Body->SetDestructibleMesh(mesh);
}
