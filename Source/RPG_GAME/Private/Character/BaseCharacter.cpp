// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include <Item/Weapon.h>
#include <Components/BoxComponent.h>
#include "Animation/AnimMontage.h"
#include "Components/AttribtueComponent.h"
#include <Kismet/GameplayStatics.h>

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Attributes = CreateDefaultSubobject<UAttribtueComponent>(TEXT("Attribute"));
	
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint, UAnimMontage* Montage)
{
	// 캐릭터의 전방 벡터 얻기
	const FVector Forward = GetActorForwardVector();

	// 충격 지점의 Z 좌표를 적의 위치에 맞춥니다
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);

	// 충격 지점으로부터 적의 위치까지의 벡터를 정규화합니다
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward와 ToHit 벡터 사이의 각도(코사인 값)를 계산합니다
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// 역 코사인(cos(theta)의 역함수)을 사용하여 각도를 얻습니다
	double Theta = FMath::Acos(CosTheta);

	// 라디안 값을 도(degree)로 변환합니다
	Theta = FMath::RadiansToDegrees(Theta);

	// CrossProduct의 Z 값이 음수인 경우, Theta를 음수로 변경합니다
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

	PlayMontage(Section, Montage);
}

void ABaseCharacter::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	if (Attributes && Attributes->IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation(), HitMontage);
	}
	else
	{
		Die();
	}
	
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactPoint);
	}
}

void ABaseCharacter::PlayAttackMontage(UAnimMontage* Montage, TArray<FName> Section)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (Montage && AnimInstance && Section.Num() > 0)
	{
		const int32 Index = Section.Num() - 1;
		const int32 SectionName = FMath::RandRange(0, Index);
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(Section[SectionName], Montage);
	}
}

void ABaseCharacter::PlayMontage(const FName Section, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(Section, Montage);
	}
}

void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
	PlayMontage(DeathMontageSection(), DeathMontage);
	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorEnableCollision(false);
	}
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25, AttackMontage);
	}

}

void ABaseCharacter::SetCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetBox())
	{
		EquippedWeapon->GetBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActor.Empty(); // 충돌배우를 다시 비워줌
	}
}

void ABaseCharacter::AttackEnd()
{

}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe =( Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	//DrawDebugSphere(GetWorld(), (CombatTargetLocation + TargetToMe), 25.f, 12.f, FColor::White, false, 5.f);
	return CombatTargetLocation + TargetToMe;

}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

FName ABaseCharacter::DeathMontageSection()
{
		const int32 Section = FMath::RandRange(0, 1);
		FName SectionName;

		switch (Section)
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



