// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include <Item/Weapon.h>
#include <Components/BoxComponent.h>

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::PlayAttackMontage(UAnimMontage* Montage, FName Section)
{

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (Weapon && Weapon->GetBox())
	{
		Weapon->GetBox()->SetCollisionEnabled(CollisionEnabled);
		Weapon->IgnoreActor.Empty(); // 충돌배우를 다시 비워줌
	}
}

