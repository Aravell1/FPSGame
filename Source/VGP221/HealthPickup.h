// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCollectible.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class VGP221_API AHealthPickup : public ABaseCollectible
{
	GENERATED_BODY()

public:
	AHealthPickup();

	virtual void OnCollect() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* MeshComponent;

	float HealValue = 5;
	
};
