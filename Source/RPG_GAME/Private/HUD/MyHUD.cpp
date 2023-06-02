// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "HUD/MainCharacterOverlay.h"
void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* MyController = World->GetFirstPlayerController();
		if (MyController && MyOveraly)
		{
			Overlay = CreateWidget<UMainCharacterOverlay>(MyController, MyOveraly);
			Overlay->AddToViewport();
		}
	}
	
	
}
