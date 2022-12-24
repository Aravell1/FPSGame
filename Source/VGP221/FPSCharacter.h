// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "FPSProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "VGP221GameModeBase.h"

#include "FPSCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerBroadcastDelegate);

UCLASS()
class VGP221_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
		float Health = 20;

	UFUNCTION()
		virtual void TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void StartFiring();

	UFUNCTION()
		void StopFiring();

	UFUNCTION()
		void StartReloading();

	UFUNCTION()
		void StopReloading();

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	UFUNCTION(BlueprintPure)
		float GetHealth();

	UFUNCTION(BlueprintPure)
		float GetMaxHealth();

	UFUNCTION(BlueprintPure)
		bool GetWeaponJammed();

	UFUNCTION(BlueprintPure)
		float GetJamReleaseTimer();

	UFUNCTION(BlueprintPure)
		float GetJamReleaseTime();

	UFUNCTION(BlueprintPure)
		float GetJamTimer();

	UFUNCTION(BlueprintPure)
		float GetWeaponJamTime();

	UFUNCTION(BlueprintPure)
		float GetAmmoCount();

	UFUNCTION(BlueprintPure)
		float GetMaxAmmo();

	UFUNCTION(BlueprintPure)
		float GetReloadTime();

	UFUNCTION(BlueprintPure)
		float GetReloadTimer();

	UFUNCTION(BlueprintPure)
		bool GetDamageBuffed();

	UFUNCTION(BlueprintPure)
		float GetBuffTimer();

	UFUNCTION(BlueprintPure)
		float GetMaxBuffTime();

	void HealPlayer(int HealthValue);

	void SetDamageBuff(bool IsBuffed);

	UPROPERTY(BlueprintAssignable)
		FPlayerBroadcastDelegate UpdateHealthBar;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FPSMesh;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		UStaticMeshComponent* FirePoint;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

private:
	
	void Fire();
	
	float MaxHealth = 20;

	float BaseDamage = 1;
	bool DamageBuff = false;
	float BuffTimer = 0;
	float MaxBuffTime = 20;
	
	float AmmoCount = 20;
	float MaxAmmo = 20;
	
	bool Reloading = false;
	float ReloadTime = 1;
	float ReloadTimer = 0;
	
	bool WeaponJammed = false;
	float WeaponJamTime = 5;
	float JamTimer = 0;
	
	float JamReleaseTime = 5;
	float JamReleaseTimer = 5;
	
	bool CoolingDown = false;
	float CooldownTime = 2;
	float CooldownTimer = 2;

	bool Firing = false;
	bool CanFire = true;
	float TimeBetweenShots = 0.5f;
	float ShootTimer = 0;


};
