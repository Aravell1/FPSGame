// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionComponent->SetGenerateOverlapEvents(true);
		CollisionComponent->SetSimulatePhysics(true);
		CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;
		
	}
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Mesh/Projectile/Sphere.Sphere"));

		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}

		ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
		ProjectileMeshComponent->SetupAttachment(RootComponent);
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
	}

	InitialLifeSpan = 10.0f;
}

void ABaseProjectile::SetDamage(float D)
{
	Damage = D;
}

void ABaseProjectile::SetHasBlastRadius(bool B, float BR)
{
	HasBlastRadius = B;
	BlastRadius = BR;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasBlastRadius)
	{
		if (Cast<AFPSCharacter>(OtherActor))
			UGameplayStatics::ApplyDamage(OtherActor, Damage, NULL, NULL, NULL);
		else
		{
			TArray<FHitResult> HitActors;
			TArray<AActor*> IgnoreActors;
			if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint, 
				BlastRadius, ETraceTypeQuery::TraceTypeQuery5, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitActors, true))
			{
				if (HitActors.Num() > 0)
				{
					for (int i = 0; i < HitActors.Num(); i++)
					{
						if (Cast<AFPSCharacter>(HitActors[i].GetActor()))
						{
							UGameplayStatics::ApplyDamage(HitActors[i].GetActor(), Damage, NULL, NULL, NULL);
							break;
						}
					}
				}
			}
		}
	}
	else if (Cast<AFPSCharacter>(OtherActor))
		UGameplayStatics::ApplyDamage(OtherActor, Damage, NULL, NULL, NULL);

	Destroy();
}

