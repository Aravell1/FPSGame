// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretSpawner.h"

// Sets default values
ATurretSpawner::ATurretSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATurretSpawner::SpawnTurret(bool Random, int SpawnIndex)
{
	FHitResult Hit;
	FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 300);
	FCollisionQueryParams Params;
	FActorSpawnParameters SpawnInfo;
	TSubclassOf<ABaseTurret> TurretClass;

	if (SpawnShooterOnly)
	{
		TurretClass = TurretTypes[0];
	}
	else if (Random)
	{
		TurretClass = TurretTypes[FMath::RandRange(0, TurretTypes.Num() - 1)];
	}
	else
	{
		TurretClass = TurretTypes[SpawnIndex];
	}

	ABaseTurret* Turret = GetWorld()->SpawnActor<ABaseTurret>(TurretClass, SpawnLocation, GetActorRotation(), SpawnInfo);
	Params.AddIgnoredActor(Turret);
	SpawnLocation = Turret->GetActorLocation();
	GetWorld()->LineTraceSingleByChannel(OUT Hit, SpawnLocation, FVector(SpawnLocation.X, SpawnLocation.Y, 0),
		ECollisionChannel::ECC_Visibility, Params);
	Turret->SetActorLocation(Hit.ImpactPoint + FVector(0, 0, 50));
}

