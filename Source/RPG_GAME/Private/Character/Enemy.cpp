// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include <DrawDebugHelpers.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"
#include <Kismet/GameplayStatics.h>
#include "Components/AttribtueComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/HealthBarComponent.h>
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "./Item/Weapon.h"
#include "./Item/Soul.h"


AEnemy::AEnemy()
{
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 225.f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateEnemyBehavior();
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HideHealthBarWidget();
	SetupAIController();
	SpawnAndEquipDefaultWeapon();
	AddEnemyTag();

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
}

void AEnemy::UpdateEnemyBehavior()
{
	if (DeathPose != EDeathPose::EDP_Alive) return;

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	//타겟거리
	if (Target == nullptr) return false;
	double DistanceToTarget = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
	
	//사정거리 안에 오면 참
	return DistanceToTarget<=Radius;
}

void AEnemy::MoveToTaget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius); //반경 몇에서 멈추나
	EnemyController->MoveTo(MoveRequest);
	
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num() - 1;
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSection = FMath::RandRange(0, NumPatrolTargets);
		return ValidTargets[TargetSection];	
	}
	return nullptr;
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		HideHealthBarWidget();
		ClearAttackTimer();
		if (!IsEngaged())
		{
			StartPatrolling();
		}
	}
	else if (IsOutsideCombatRadius() && IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged())
		{
			ChaseTarget();
		}	
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 555.f;
	MoveToTaget(CombatTarget);
}


void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius)) //순찰위치가 사정거리에 있으면
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::patrolTimerFinished, WaitTime);
		//DrawDebugSphere(GetWorld(), PatrolTarget->GetActorLocation(), 200.f, 12.f, FColor::White, false, 10.f);
	}
}

void AEnemy::HideHealthBarWidget()
{
	if (HealthBarWidget && Attributes)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::SetupAIController()
{
	EnemyController = Cast<AAIController>(GetController());
	if (EnemyController)
	{
		MoveToTaget(PatrolTarget);
	}
}

void AEnemy::SpawnAndEquipDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* Weapon = World->SpawnActor<AWeapon>(WeaponClass);
		Weapon->ItemEquip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = Weapon;
	}
}

void AEnemy::AddEnemyTag()
{
	Tags.Add(AITag);
}

void AEnemy::ClearAttackTimer()
{
	CombatTarget = nullptr;

	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::IsChasing()
{
	return  EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = 225.f;
	MoveToTaget(PatrolTarget);
}


void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::Die()
{
	Super::Die();

	IsDead();
	ClearAttackTimer();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	SpawnSoul();
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AEnemy::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSoul(Attributes->GetSoul());
			SpawnedSoul->SetOwner(this);
		}
	}
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer,this, &AEnemy::Attack,AttackTime);
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		EnemyState != EEnemyState::EES_Engaged &&
		SeenPawn->ActorHasTag(FName("EngagebleTarget"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
	
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::PlayAttackMontage(UAnimMontage* Montage, TArray<FName> Section)
{
	Super::PlayAttackMontage(Montage, Section);
}
void AEnemy::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage(AttackMontage, MontageSection);
}
void AEnemy::patrolTimerFinished()
{
	MoveToTaget(PatrolTarget);
}

void AEnemy::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit(ImpactPoint,Hitter);

	GetWorldTimerManager().ClearTimer(PatrolTimer);
	GetWorldTimerManager().ClearTimer(AttackTimer);
	StopAttackMontage();
}
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());	
	}
	CombatTarget = EventInstigator->GetPawn();
	if (InTargetRange(CombatTarget, AttackRadius))
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (!InTargetRange(CombatTarget, AttackRadius))
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 555.f;
		MoveToTaget(CombatTarget);
	}
	return DamageAmount;
}
