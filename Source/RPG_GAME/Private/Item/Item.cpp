// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <./Character/MyCharacter.h>

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);

	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ItemState != EItemState::EIS_Equipped)
	{
		RunningTime += DeltaTime;
		AddActorWorldOffset(FVector(0.f, 0.f, MySin()));
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	//콜리전 이벤트 동적활당
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::SphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::SphereEndOverlap);

}

void AItem::SphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);

	if (MyCharacter)
	{
		MyCharacter->SetItme(this);
	}
	
}

void AItem::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->SetItme(nullptr);
	}
}

float AItem::MySin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

