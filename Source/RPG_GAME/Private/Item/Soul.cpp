// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Soul.h"
#include <Interface/PickupInterface.h>
#include <NiagaraFunctionLibrary.h>

void ASoul::SphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->AddSouls(this);
		if (PickupEffec)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				PickupEffec,
				GetActorLocation()
			);
		}
		Destroy();
	}
	}
	
