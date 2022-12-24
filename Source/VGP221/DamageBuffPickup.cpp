// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBuffPickup.h"

ADamageBuffPickup::ADamageBuffPickup()
{
	if (!MeshComponent)
	{
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	}
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
	}

	RotationRate = 45;
}

void ADamageBuffPickup::OnCollect()
{
	PlayerRef->SetDamageBuff(true);
	Destroy();
}

void ADamageBuffPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0, RotationRate * DeltaTime, 0));
}