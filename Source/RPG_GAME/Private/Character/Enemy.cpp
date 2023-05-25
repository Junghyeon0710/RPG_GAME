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


AEnemy::AEnemy()
{
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	Attributes = CreateDefaultSubobject<UAttribtueComponent>(TEXT("Attribute"));
	
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
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
	
}
	
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget && Attributes)
	{
		HealthBarWidget->SetVisibility(false);
	}
	EnemyController = Cast<AAIController>(GetController());
	MoveToTaget(PatrolTarget);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

}

void AEnemy::PlayMontage(const FName Section, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(Section, Montage);
	}
}

FName AEnemy::DeathMontageSection()
{ 
	const int32 Section = FMath::RandRange(0, 1);
	FName SectionName;

	switch(Section)
	{
	case 0:
		SectionName = FName("Death1");
		DeathPose = EDeathPose::EDP_Death1;
		break;
	case 1:
		SectionName = FName("Death2");
		DeathPose = EDeathPose::EDP_Death2;
		break;
	default:
			break;
	}
	return SectionName;
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
	MoveRequest.SetAcceptanceRadius(15.f);
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
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 225.f;
		MoveToTaget(PatrolTarget);
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 555.f;
		MoveToTaget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
		// Attack Montage
	}
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

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) return;
	if (SeenPawn->ActorHasTag(FName("MyCharacter")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		CombatTarget = SeenPawn;
		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTaget(CombatTarget);
		}
	}
}

void AEnemy::patrolTimerFinished()
{
	MoveToTaget(PatrolTarget);
}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	if (Attributes && Attributes->IsAlive())
		DirectionalHitReact(ImpactPoint);
	else
	{
		PlayMontage(DeathMontageSection(), DeathMontage);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetLifeSpan(5.f);
	}
	
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitParticle, ImpactPoint);
	}
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
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 555.f;
	MoveToTaget(CombatTarget);
	return DamageAmount;
}
void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("Back");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("Front");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("Left");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("Right");
	}
	PlayMontage(Section,HitMontage);
}