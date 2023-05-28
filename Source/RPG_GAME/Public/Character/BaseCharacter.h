// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "../Interface/MyInterface.h"
#include "BaseCharacter.generated.h"



UCLASS()
class RPG_GAME_API ABaseCharacter : public ACharacter , public IMyInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	void DirectionalHitReact(const FVector& ImpactPoint, UAnimMontage* Montage);
protected:

	virtual void BeginPlay() override;
	virtual void PlayAttackMontage(UAnimMontage* Montage,  TArray<FName> Section); //공격 몽타주
	virtual void PlayMontage(const FName Section, UAnimMontage* Montage);
	UFUNCTION(BlueprintCallable)
	void SetCollision(ECollisionEnabled::Type CollisionEnabled);
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();


	UPROPERTY(VisibleAnywhere, Category = "Item")
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> MontageSection;

	UPROPERTY(EditAnywhere, Category = "파티클")
	class UParticleSystem* HitParticle;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	ECharacterAnimationState CharacterAnimaionState = ECharacterAnimationState::EAS_None;
};
