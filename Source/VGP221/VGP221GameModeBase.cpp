// Copyright Epic Games, Inc. All Rights Reserved.


#include "VGP221GameModeBase.h"

AVGP221GameModeBase::AVGP221GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVGP221GameModeBase::SetTurretCount(int countToAdd)
{
	TurretCount += countToAdd;
	if (TurretCount <= 0)
	{
		StopTimer = true;
		GameWon = true;
		CallGameEnd.Broadcast();
	}
}

void AVGP221GameModeBase::Tick(float DeltaTime)
{
	if (Timer > 0 && !StopTimer)
	{
		Timer -= DeltaTime;
		if (Timer < 0)
		{
			Timer = 0;
			StopTimer = true;
			GameWon = false;
			CallGameEnd.Broadcast();
		}
	}
}
