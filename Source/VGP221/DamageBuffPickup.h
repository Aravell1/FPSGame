// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCollectible.h"
#include "DamageBuffPickup.generated.h"

/**
 * 
 */
UCLASS()
class VGP221_API ADamageBuffPickup : public ABaseCollectible
{
	GENERATED_BODY()

public:
	ADamageBuffPickup();

	virtual void OnCollect() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* MeshComponent;
	
};
