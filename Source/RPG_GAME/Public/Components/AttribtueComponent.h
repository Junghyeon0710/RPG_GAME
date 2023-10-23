// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttribtueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_GAME_API UAttribtueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttribtueComponent();

private:
	
	UPROPERTY(EditAnywhere, Category = "체력")
	float Health;

	UPROPERTY(EditAnywhere,Category = "체력")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "체력")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "체력")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere,Category = "Gold")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Gold")
	int32 Soul;

	UPROPERTY(EditAnywhere, Category = "체력")
	float DodgeCost =14.f;

	UPROPERTY(EditAnywhere, Category = "체력")
	float StaminaRegenRate = 8.f; //초당 마나 충전

public:
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	bool IsAlive();
	void AddSouls(int32 NumberofSouls);
	void AddGold(int32 NumberofGold);
	void RegenStamina(float DeltaTime);
	float GetHealthPercent();
	float GetStaminaPercent();
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSoul() const { return Soul; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
};
