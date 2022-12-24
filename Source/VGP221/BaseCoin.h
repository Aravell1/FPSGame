// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCollectible.h"
#include "VGP221GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCoin.generated.h"

UCLASS()
class VGP221_API ABaseCoin : public ABaseCollectible
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCoin();

	float Points = 5;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnCollect() override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CoinMesh;

	FTimerHandle DeathTimerHandle;
	AVGP221GameModeBase* GameMode;

	void DeathTimerComplete();
};
