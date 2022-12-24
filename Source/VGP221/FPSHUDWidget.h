// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "FPSHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class VGP221_API UFPSHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Make sure bound object in blueprint has same name
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		//UButton* PlayButton;
		UTextBlock* TextBlock;

	UFUNCTION()
		void SetTextBlock(int number);

//protected:
//	virtual void NativeConstruct() override;
//
//	UFUNCTION()
//	void OnButtonClick();

};
