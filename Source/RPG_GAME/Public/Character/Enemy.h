// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "../Interface/MyInterface.h"
#include "CharacterTypes.h"
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

	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* DeathMontage;
	void DirectionalHitReact(const FVector& ImpactPoint);

	void PlayMontage(const FName Section, UAnimMontage* Montage);
	bool InTargetRange(AActor* Target, double Radius);

	FName DeathMontageSection();

	UPROPERTY(EditAnywhere, Category = "파티클")
	class UParticleSystem* HitParticle;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
private:

	UPROPERTY(VisibleAnywhere)
	class UAttribtueComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "반경")
	float CombatRadius = 500.f;

	/** Navigation*/

	class AAIController* EnemyController;
	// 시작 순찰 위치
	UPROPERTY(EditInstanceOnly,Category = "Ai Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "Ai Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "반경")
	float PatrolRadius = 200.f;
};
