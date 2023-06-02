// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainCharacterOverlay.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UMainCharacterOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UMainCharacterOverlay::SetStaminaProgressBarPercent(float Percent)
{
	if (StanminaProgressBar)
	{
		StanminaProgressBar->SetPercent(Percent);
	}
}

void UMainCharacterOverlay::SetGold(int32 Gold)
{
	if (GoldCount)
	{
		GoldCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
}

void UMainCharacterOverlay::SetSoul(int32 Souls)
{
	if (SoulCount)
	{
		SoulCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
