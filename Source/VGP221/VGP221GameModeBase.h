// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSHUDWidget.h"
#include "VGP221GameModeBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEndDelegate);

UCLASS()
class VGP221_API AVGP221GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVGP221GameModeBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float Timer = 120;

	void SetTurretCount(int countToAdd);
	

protected:

	virtual void Tick(float DeltaTime) override;

public:	

	UPROPERTY(BlueprintAssignable)
		FGameEndDelegate CallGameEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int TurretCount = 0;

	bool StopTimer = false;

	UPROPERTY(BlueprintReadOnly)
		bool GameWon = false;

};
