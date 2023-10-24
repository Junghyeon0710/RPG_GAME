// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/treasure.h"
#include <Interface/PickupInterface.h>
// Sets default values
Atreasure::Atreasure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void Atreasure::BeginPlay()
{
	Super::BeginPlay();
	
}

void Atreasure::SphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->AddGold(this);
		Destroy();
	}
}


