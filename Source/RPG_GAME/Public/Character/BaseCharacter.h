// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "BaseCharacter.generated.h"

UCLASS()
class RPG_GAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCollision(ECollisionEnabled::Type CollisionEnabled);

protected:

	virtual void BeginPlay() override;
	virtual void PlayAttackMontage();

	UPROPERTY(VisibleAnywhere, Category = "Item")
		class AWeapon* Weapon;
};
