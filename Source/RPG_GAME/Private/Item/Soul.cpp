// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Soul.h"
#include <Interface/PickupInterface.h>
#include <NiagaraFunctionLibrary.h>
#include "Kismet/KismetSystemLibrary.h"

void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveSoul(DeltaTime);
}

void ASoul::BeginPlay()
{
	Super::BeginPlay();

	CalculateDesiredZOffset();
}

void ASoul::SphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleItemPickup(OtherActor);
}

void ASoul::MoveSoul(float DeltaTime)
{
	const double LocationZ = GetActorLocation().Z;
	if (LocationZ > DesiredZ)
	{
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		AddActorWorldOffset(DeltaLocation);
	}
}

void ASoul::HandleItemPickup(AActor* OtherActor)
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

void ASoul::CalculateDesiredZOffset()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true);

	DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}
