// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUDWidget.h"

//void UFPSHUDWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	PlayButton->OnClicked.AddUniqueDynamic(this, &UFPSHUDWidget::OnButtonClick);
//}
//
//
//void UFPSHUDWidget::OnButtonClick()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Button Clicked"));
//}

void UFPSHUDWidget::SetTextBlock(int number)
{
	if (TextBlock)
	{
		FText finalString = FText::FromString("Number: " + FString::FromInt(number));
		TextBlock->SetText(finalString);
	}
}
