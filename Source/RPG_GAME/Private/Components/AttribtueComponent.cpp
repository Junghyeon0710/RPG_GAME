// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttribtueComponent.h"

UAttribtueComponent::UAttribtueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAttribtueComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UAttribtueComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

float UAttribtueComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttribtueComponent::GetStaminaPercent()
{
	return Stamina/MaxStamina;
}

bool UAttribtueComponent::IsAlive()
{
	return Health > 0.f;
}

void UAttribtueComponent::AddSouls(int32 NumberofSouls)
{
	Soul += NumberofSouls;
}

void UAttribtueComponent::AddGold(int32 NumberofGold)
{
	Gold += NumberofGold;
}

void UAttribtueComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}


