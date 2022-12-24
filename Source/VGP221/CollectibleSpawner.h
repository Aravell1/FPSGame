// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCollectible.h"
#include "CollectibleSpawner.generated.h"

UCLASS()
class VGP221_API ACollectibleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectibleSpawner();

	UFUNCTION(BlueprintCallable)
		void SpawnCollectible(bool Random = true, int SpawnIndex = 0);

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<ABaseCollectible>> PickupTypes;


};
