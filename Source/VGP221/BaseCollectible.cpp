// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCollectible.h"

// Sets default values
ABaseCollectible::ABaseCollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	}
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseCollectible::BeginOverlap);
		CollisionComponent->InitSphereRadius(30.0f);
		RootComponent = CollisionComponent;
	}
}

// Called when the game starts or when spawned
void ABaseCollectible::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerRef = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ABaseCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCollectible::OnCollect()
{
	UE_LOG(LogTemp, Warning, TEXT("BaseCollectible: OnCollect()"));
}

void ABaseCollectible::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerRef)
		OnCollect();
}

