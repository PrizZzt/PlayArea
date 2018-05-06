#include "TDirector.h"

ATDirector::ATDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ATDirector::BeginPlay()
{
	Super::BeginPlay();
}
