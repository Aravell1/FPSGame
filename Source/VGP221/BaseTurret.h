// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h" 
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DecalActor.h"
#include "FPSCharacter.h"
#include "BaseTurret.generated.h"

UENUM()
enum class TurretType
{
	HighArc	UMETA(DisplayName = "High Arcing Launcher"),
	LowArc	UMETA(DisplayName = "Low Arcing Launcher")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBroadcastDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBroadcastDelegateBool);

UCLASS()
class VGP221_API ABaseTurret : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		TurretType ArcStyle = TurretType::HighArc;

	UPROPERTY(EditDefaultsOnly)
		float LaunchForce = 3000;

	UPROPERTY(EditDefaultsOnly)
		float LaunchDelay = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float MaxDistance = 3000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float MaxHealth = 5;

	UPROPERTY(EditDefaultsOnly)
		float BaseDamage = 3;

	UPROPERTY(EditDefaultsOnly)
		bool HasBlastRadius = true;

	UPROPERTY(EditDefaultsOnly)
		float BlastRadius = 200;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* MainMesh;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Launcher;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* LauncherMesh;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* LaunchPoint;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* Collider;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABaseProjectile> Projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool canSeePlayer = true;

	UFUNCTION()
		virtual void TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
		void RemoveDecal(AActor* DestroyedActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Health;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADecalActor> TargetDecal;

private:

	void LaunchProjectile();
	double CalculateLaunchAngle();

	bool CanLaunch = true;
	float LaunchTimer = 0;

	
	UPROPERTY(BlueprintAssignable)
		FBroadcastDelegate UpdateHealthBar;

	UPROPERTY(BlueprintAssignable)
		FBroadcastDelegateBool UpdateHealthBarVisibility;

	TArray<ADecalActor*> SpawnedDecals;
	AFPSCharacter* PlayerRef;

};
