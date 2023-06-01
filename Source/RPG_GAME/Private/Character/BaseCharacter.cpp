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
	PlayMontage(Section, Montage);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	if (Attributes && Attributes->IsAlive() && Hitter)
		DirectionalHitReact(Hitter->GetActorLocation(), HitMontage);
	else
	{
		PlayMontage(DeathMontageSection(), DeathMontage);
		EquippedWeapon->SetActorEnableCollision(false);
	}
	
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitParticle, ImpactPoint);
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



