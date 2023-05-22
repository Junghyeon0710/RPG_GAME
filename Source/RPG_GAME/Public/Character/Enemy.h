// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "../Interface/MyInterface.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class RPG_GAME_API AEnemy : public ABaseCharacter, public IMyInterface
{
	GENERATED_BODY()
public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category=Montage)
	class UAnimMontage* HitMontage;
	void DirectionalHitReact(const FVector& ImpactPoint);

	void PlayMontage(const FName Section);

	UPROPERTY(EditAnywhere, Category = "파티클")
	class UParticleSystem* HitParticle;
private:

	UPROPERTY(VisibleAnywhere)
	class UAttribtueComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;
};
