// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BrakedActor.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>
#include <../Public/Item/treasure.h>

ABrakedActor::ABrakedActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABrakedActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABrakedActor::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBarekd) return;

	bBarekd = true;

	SpawnTreasure();
	PlayHitSound(ImpactPoint);
}

void ABrakedActor::SpawnTreasure()
{
	UWorld* World = GetWorld();
	if (World && TreasureClass.Num() > 0)
	{
		const int32 Treasure = TreasureClass.Num() - 1;
		const int32 Index = FMath::RandRange(0, Treasure);

		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<Atreasure>(TreasureClass[Index], Location, GetActorRotation());
	}
}

void ABrakedActor::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}
