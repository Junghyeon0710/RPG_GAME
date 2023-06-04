// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttribtueComponent.h"


// Sets default values for this component's properties
UAttribtueComponent::UAttribtueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttribtueComponent::BeginPlay()
{
	Super::BeginPlay();

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


void UAttribtueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

