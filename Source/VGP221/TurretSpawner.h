// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTurret.h"
#include "TurretSpawner.generated.h"

UCLASS()
class VGP221_API ATurretSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretSpawner();

	UFUNCTION(BlueprintCallable)
		void SpawnTurret(bool Random = true, int SpawnIndex = 0);

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<ABaseTurret>> TurretTypes;

	UPROPERTY(EditAnywhere)
		bool SpawnShooterOnly = false;

};
