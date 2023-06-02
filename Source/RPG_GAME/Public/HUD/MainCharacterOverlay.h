// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class RPG_GAME_API UMainCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaProgressBarPercent(float Percent);
	void SetGold(int32 Gold);
	void SetSoul(int32 Souls);
private:

	UPROPERTY(meta =(bindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (bindWidget))
	class UProgressBar* StanminaProgressBar;

	UPROPERTY(meta = (bindWidget))
	class UTextBlock* GoldCount;

	UPROPERTY(meta = (bindWidget))
	class UTextBlock* SoulCount;
};
