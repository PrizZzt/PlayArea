// Fill out your copyright notice in the Description page of Project Settings.

#include "TDestructibleObject.h"

ATDestructibleObject::ATDestructibleObject()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Body = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);

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
	Body->ApplyDamage(100, GetActorLocation(), GetActorLocation(), 5000);
}

void ATDestructibleObject::Renew()
{
	Body->SetDestructibleMesh(mesh);
}

void ATDestructibleObject::SetRotation(FRotator &_rotation)
{
	Body->SetWorldRotation(_rotation);
}
