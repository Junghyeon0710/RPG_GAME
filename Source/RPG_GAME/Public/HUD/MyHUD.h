// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_GAME_API AMyHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	void CreateMainCharacterOverlay();

private:

	UPROPERTY(EditDefaultsOnly,Category = Class)
	TSubclassOf<class UMainCharacterOverlay> MyOveraly;

	UPROPERTY()
	class UMainCharacterOverlay* Overlay;

public:
	FORCEINLINE UMainCharacterOverlay* GetMyOverlay() const { return Overlay; }
};
