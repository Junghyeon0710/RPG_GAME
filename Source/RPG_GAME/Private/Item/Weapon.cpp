// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon.h"

AWeapon::AWeapon()
{

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::SphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::SphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::ItemEquip(USceneComponent* Parent)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true);
	Mesh->AttachToComponent(Parent, TransformRules, FName("RightHandSocket"));
}
