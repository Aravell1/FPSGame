// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTurret.h"

// Sets default values
ABaseTurret::ABaseTurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!Collider)
	{
		Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
		RootComponent = Collider;
	}

	if (!MainMesh)
	{
		MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Primary Mesh"));
		MainMesh->SetupAttachment(Collider);
	}

	if (!Launcher)
	{
		Launcher = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launcher Body"));
		Launcher->SetupAttachment(Collider);
	}

	if (!LauncherMesh)
	{
		LauncherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launcher Gun"));
		LauncherMesh->SetupAttachment(Launcher);
	}

	if (!LaunchPoint)
	{
		LaunchPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launch Location"));
		LaunchPoint->SetupAttachment(LauncherMesh);
	}


	if (Health != MaxHealth)
		Health = MaxHealth;

	OnTakeAnyDamage.AddDynamic(this, &ABaseTurret::TakeAnyDamage);
}

// Called when the game starts or when spawned
void ABaseTurret::BeginPlay()
{
	Super::BeginPlay();
	
	if (!PlayerRef)
	{
		PlayerRef = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	if (Health != MaxHealth)
		Health = MaxHealth;

	Cast<AVGP221GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetTurretCount(1);
}

void ABaseTurret::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	for (int i = 0; i < SpawnedDecals.Num(); i++)
	{
		SpawnedDecals[i]->Destroy();
	}
}

// Called every frame
void ABaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanLaunch)
	{
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		FVector Start = LaunchPoint->GetComponentLocation();
		FVector End = LaunchPoint->GetComponentLocation() + (PlayerRef->GetActorLocation() - LaunchPoint->GetComponentLocation()).GetSafeNormal() * MaxDistance;

		canSeePlayer = GetWorld()->LineTraceSingleByChannel(OUT Hit, Start, End, ECollisionChannel::ECC_Camera, Params);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red);

		if (FVector::Distance(GetActorLocation(), PlayerRef->GetActorLocation()) < MaxDistance && canSeePlayer)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Hit.GetActor()->GetName());
			if (Hit.GetActor() == PlayerRef)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Shooting"));
				CanLaunch = false;
				Launcher->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(Launcher->GetComponentLocation(), PlayerRef->GetActorLocation()));
				LaunchProjectile();

				if (ArcStyle == TurretType::HighArc && TargetDecal)
				{
					FActorSpawnParameters SpawnInfo;
					ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(TargetDecal, 
						PlayerRef->GetActorLocation(), PlayerRef->GetActorRotation() + FRotator(0, 45, 0), SpawnInfo);
					SpawnedDecals.Add(Decal);
				}
			}
		}
	}

	if (CanLaunch == false)
	{
		LaunchTimer += DeltaTime;
		if (LaunchTimer >= LaunchDelay)
		{
			LaunchTimer = 0;
			CanLaunch = true;
		}
	}

}

void ABaseTurret::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;
	UpdateHealthBar.Broadcast();
	if (Health <= 0)
	{
		Cast<AVGP221GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetTurretCount(-1);
		Destroy();
	}
}

void ABaseTurret::RemoveDecal(AActor* DestroyedActor)
{
	SpawnedDecals[0]->Destroy();
	SpawnedDecals.RemoveAt(0, 1, true);
}

void ABaseTurret::LaunchProjectile()
{
	FRotator NewRotation = Launcher->GetComponentRotation();
	Launcher->SetRelativeRotation(NewRotation);
	NewRotation.Pitch = CalculateLaunchAngle();
	FVector SpawnLocation = LaunchPoint->GetComponentLocation();
	Launcher->SetRelativeRotation(NewRotation);

	FActorSpawnParameters SpawnInfo;
	ABaseProjectile* Bullet = GetWorld()->SpawnActor<ABaseProjectile>(Projectile, SpawnLocation, NewRotation, SpawnInfo);
	Bullet->ProjectileMovementComponent->MaxSpeed = LaunchForce * Bullet->CollisionComponent->GetMass();
	Bullet->SetDamage(BaseDamage);
	Bullet->SetHasBlastRadius(HasBlastRadius, BlastRadius);
	Bullet->CollisionComponent->AddImpulse(Bullet->GetActorForwardVector() * LaunchForce * Bullet->CollisionComponent->GetMass());
	
	if (ArcStyle == TurretType::HighArc && TargetDecal)
		Bullet->OnDestroyed.AddDynamic(this, &ABaseTurret::RemoveDecal);
}

double ABaseTurret::CalculateLaunchAngle()
{
	FVector TargetPos = PlayerRef->GetActorLocation();
	FVector SourcePos = LaunchPoint->GetComponentLocation();

	FVector Diff = TargetPos - SourcePos;
	FVector DiffXY = FVector(Diff.X, Diff.Y, 0);
	double HDistance = DiffXY.Length() / 100.0f;
	double ZDistance = Diff.Z / 100.0f;
	double Speed = LaunchForce / 100.0f;
	double Speed2 = Speed * Speed;
	double Speed4 = Speed2 * Speed2;
	double Gravity = GetWorld()->GetGravityZ() * -1 / 100.0f;
	double GX = Gravity * HDistance;
	double Root = Speed4 - Gravity * (Gravity * HDistance * HDistance + 2 * ZDistance * Speed2);

	if (Root < 0)
		return 0;

	Root = FMath::Sqrt(Root);

	double Angle01 = FMath::RadiansToDegrees(FMath::Atan2(Speed2 - Root, GX));
	double Angle02 = FMath::RadiansToDegrees(FMath::Atan2(Speed2 + Root, GX));

	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("X: " + FString::SanitizeFloat(HDistance)));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Y: " + FString::SanitizeFloat(ZDistance)));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Speed: " + FString::SanitizeFloat(Speed)));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gravity: " + FString::SanitizeFloat(Gravity)));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A1: " + FString::SanitizeFloat(Angle01)));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A2: " + FString::SanitizeFloat(Angle02)));*/

	if (ArcStyle == TurretType::HighArc)
		return FMath::Max(Angle01, Angle02);
	else
		return FMath::Min(Angle01, Angle02);

}

