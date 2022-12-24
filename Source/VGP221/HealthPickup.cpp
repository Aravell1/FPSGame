// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

AHealthPickup::AHealthPickup()
{
	if (!MeshComponent)
	{
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	}
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
	}

	RotationRate = 22.5f;
}

void AHealthPickup::OnCollect()
{
	PlayerRef->HealPlayer(HealValue);
	Destroy();
}

void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0, RotationRate * DeltaTime, 0));
}
