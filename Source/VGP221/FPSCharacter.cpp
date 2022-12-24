// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set Up Camera
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	//Set Up FPS Mesh
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	FPSMesh->SetOnlyOwnerSee(true);
	FPSMesh->SetupAttachment(FPSCameraComponent);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	if (!Weapon)
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
		Weapon->SetupAttachment(FPSMesh, FName("b_RightHandSocket"));
	}
	if (!FirePoint)
	{
		FirePoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fire Point"));
		FirePoint->SetupAttachment(Weapon);
	}

	//Hide Third Person Mesh
	GetMesh()->SetOwnerNoSee(true);

	OnTakeAnyDamage.AddDynamic(this, &AFPSCharacter::TakeAnyDamage);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (Health != MaxHealth)
		Health = MaxHealth;
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Firing && AmmoCount > 0 && !Reloading)
	{
		if (!WeaponJammed)
		{
			JamTimer += DeltaTime;
			if (JamTimer >= WeaponJamTime)
			{
				WeaponJammed = true;
				JamTimer = 0;
			}
		}

		if (CanFire && !WeaponJammed)
		{
			Fire();
			CanFire = false;
		}
	}
	else if (!Firing && CoolingDown)
	{
		if (!WeaponJammed)
		{
			JamTimer += DeltaTime;
			if (JamTimer >= WeaponJamTime)
			{
				WeaponJammed = true;
				JamTimer = 0;
			}
		}
	}

	if (CoolingDown)
	{
		CooldownTimer -= DeltaTime;
		if (CooldownTimer <= 0)
		{
			JamTimer = 0;
			CoolingDown = false;
			CooldownTimer = CooldownTime;
		}
	}

	if (WeaponJammed)
	{
		JamReleaseTimer -= DeltaTime;
		if (JamReleaseTimer <= 0)
		{
			WeaponJammed = false;
			JamReleaseTimer = JamReleaseTime;
		}
	}

	if (!CanFire)
		ShootTimer += DeltaTime;

	if (ShootTimer >= TimeBetweenShots)
	{
		CanFire = true;
		ShootTimer = 0;
	}

	if (Reloading && AmmoCount < MaxAmmo)
	{
		ReloadTimer += DeltaTime;
		if (ReloadTimer >= ReloadTime)
		{
			AmmoCount = MaxAmmo;
			ReloadTimer = 0;
			Reloading = false;
		}
	}

	if (DamageBuff)
	{
		BuffTimer += DeltaTime;
		if (BuffTimer >= MaxBuffTime)
		{
			SetDamageBuff(false);
			BuffTimer = 0;
		}
	}

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement Bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	//Look Bindings
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	//Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::StopFiring);

	//Reload
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::StartReloading);
	PlayerInputComponent->BindAction("Reload", IE_Released, this, &AFPSCharacter::StopReloading);
}

void AFPSCharacter::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;
	UpdateHealthBar.Broadcast();
}

void AFPSCharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::StartFiring()
{
	Firing = true;
}

void AFPSCharacter::StopFiring()
{
	Firing = false;
}

void AFPSCharacter::StartReloading()
{
	if (AmmoCount < MaxAmmo)
		Reloading = true;
}

void AFPSCharacter::StopReloading()
{
	Reloading = false;
	ReloadTimer = 0;
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::Fire()
{
	if (ProjectileClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector MuzzleLocation = FirePoint->GetComponentLocation();

		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 5.0f;

		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->SetDamage(BaseDamage);
				AmmoCount--;
				CoolingDown = true;
				CooldownTimer = CooldownTime;
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

float AFPSCharacter::GetHealth()
{
	return Health;
}

float AFPSCharacter::GetMaxHealth()
{
	return MaxHealth;
}

bool AFPSCharacter::GetWeaponJammed()
{
	return WeaponJammed;
}

float AFPSCharacter::GetJamReleaseTimer()
{
	return JamReleaseTimer;
}

float AFPSCharacter::GetJamReleaseTime()
{
	return JamReleaseTime;
}

float AFPSCharacter::GetJamTimer()
{
	return JamTimer;
}

float AFPSCharacter::GetWeaponJamTime()
{
	return WeaponJamTime;
}

float AFPSCharacter::GetAmmoCount()
{
	return AmmoCount;
}

float AFPSCharacter::GetMaxAmmo()
{
	return MaxAmmo;
}

float AFPSCharacter::GetReloadTime()
{
	return ReloadTime;
}

float AFPSCharacter::GetReloadTimer()
{
	return ReloadTimer;
}

bool AFPSCharacter::GetDamageBuffed()
{
	return DamageBuff;
}

float AFPSCharacter::GetBuffTimer()
{
	return BuffTimer;
}

float AFPSCharacter::GetMaxBuffTime()
{
	return MaxBuffTime;
}

void AFPSCharacter::HealPlayer(int HealthValue)
{
	Health += HealthValue;
	FMath::Clamp(Health, 0, MaxHealth);
	UpdateHealthBar.Broadcast();
}

void AFPSCharacter::SetDamageBuff(bool IsBuffed)
{
	if (IsBuffed)
		BaseDamage = 2;
	else
		BaseDamage = 1;

	DamageBuff = IsBuffed;
}

