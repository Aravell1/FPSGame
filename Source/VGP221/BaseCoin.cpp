// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCoin.h"

// Sets default values
ABaseCoin::ABaseCoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CoinMesh)
	{
		CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
		CoinMesh->SetupAttachment(CollisionComponent);
	}

	if (!GameMode)
	{
		GameMode = Cast<AVGP221GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	RotationRate = 100;

}

// Called when the game starts or when spawned
void ABaseCoin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0, RotationRate * DeltaTime, 0));

}

void ABaseCoin::OnCollect()
{
	UE_LOG(LogTemp, Warning, TEXT("BaseCoin: OnCollect()"));

	RotationRate = 1500;

	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABaseCoin::DeathTimerComplete, 0.5f, false);
}

void ABaseCoin::DeathTimerComplete()
{
	/*if (GameMode)
		GameMode->Score += Points;*/

	Destroy();
}

