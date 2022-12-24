// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectibleSpawner.h"

// Sets default values
ACollectibleSpawner::ACollectibleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACollectibleSpawner::SpawnCollectible(bool Random, int SpawnIndex)
{
	FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 125);
	FActorSpawnParameters SpawnInfo;
	TSubclassOf<ABaseCollectible> PickupClass;

	if (Random)
	{
		PickupClass = PickupTypes[FMath::RandRange(0, PickupTypes.Num() - 1)];
	}
	else
	{
		PickupClass = PickupTypes[SpawnIndex];
	}

	GetWorld()->SpawnActor<ABaseCollectible>(PickupClass, SpawnLocation, GetActorRotation(), SpawnInfo);
}

