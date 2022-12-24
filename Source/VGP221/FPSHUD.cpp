// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"

void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture)
	{
		FVector2D Center(Canvas->ClipX / 2, Canvas->ClipY / 2);

		float CrosshairCenterWidth = CrosshairTexture->GetSurfaceWidth() / 2.0f;
		float CrosshairCenterHeight = CrosshairTexture->GetSurfaceHeight() / 2.0f;
		FVector2D CrossHairDrawPosition(Center.X - CrosshairCenterWidth, Center.Y - CrosshairCenterHeight);

		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->GetResource(), FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}
