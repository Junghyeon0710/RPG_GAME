// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class RPG_GAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	virtual void PlayAttackMontage(UAnimMontage* Montage, TArray<FName> Section) override;
	virtual void AttackEnd() override;
	virtual void Die() override;
	void DisableCapsule();
	void SpawnSoul();
	void UpdateEnemyBehavior();
	void Attack();
	void MoveToTaget(AActor* Target);
	void CheckCombatTarget();
	void ChaseTarget();
	void StartPatrolling();
	void CheckPatrolTarget();
	void HideHealthBarWidget();
	void SetupAIController();
	void SpawnAndEquipDefaultWeapon();
	void AddEnemyTag();
	void ClearAttackTimer();
	bool IsOutsideCombatRadius();
	bool IsEngaged();
	bool IsChasing();
	bool CanAttack();
	bool IsInsideAttackRadius();
	bool IsDead();
	bool IsAttacking();
	bool InTargetRange(AActor* Target, double Radius);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	void ClearPatrolTimer();

	/**combat */
	void StartAttackTimer();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere,Category ="Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere, Category = "반경")
	float CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "반경")
	float AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "반경")
	float AcceptanceRadius = 45.f;

	UPROPERTY(EditAnywhere, Category = "이름")
	FName AITag = "Enemy";

	/** Navigation*/

	class AAIController* EnemyController;
	// 시작 순찰 위치
	UPROPERTY(EditInstanceOnly,Category = "Ai Navigation",BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "Ai Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "반경")
	float PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	void patrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	float WaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	float DeathLifeSpan = 5.f;

	//Component
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere,Category=Combat)
	TSubclassOf<class ASoul> SoulClass;
	
};
