// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BrakedActor.h"
#include <DrawDebugHelpers.h>
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>
#include <../Public/Item/treasure.h>

// Sets default values
ABrakedActor::ABrakedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABrakedActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrakedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrakedActor::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	//DrawDebugSphere(GetWorld(), ImpactPoint, 25.f, 12.f, FColor::Black, false, 5.f);
	if (bBarekd) return;
	
		bBarekd = true;

		UWorld* World = GetWorld();
		if (World && TreasureClass.Num() > 0)
		{
			const int32 Treasure = TreasureClass.Num() - 1;
			const int32 Index = FMath::RandRange(0, Treasure);

			FVector Location = GetActorLocation();
			Location.Z += 75.f;
			World->SpawnActor<Atreasure>(TreasureClass[Index], Location, GetActorRotation());
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
		}
}

