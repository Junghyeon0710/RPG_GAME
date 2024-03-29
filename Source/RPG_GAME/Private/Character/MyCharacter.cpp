// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Kismet/GameplayStatics.h>
#include <Components/StaticMeshComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "./Item/Weapon.h"
#include "Animation/AnimMontage.h"
#include <Components/BoxComponent.h>
#include "Components/AttribtueComponent.h"
#include <HUD/MyHUD.h>
#include <HUD/MainCharacterOverlay.h>
#include <Components/CapsuleComponent.h>
#include <Item/Soul.h>
#include <Item/treasure.h>


AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm);
}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultMappingContext();
	AddEngageableTargetTag();
	InitializeHUD();
	InitializeMyOverlay();
}

void AMyCharacter::Die()
{
	Super::Die();
	CharacterDeadState();
	DisableMesh();
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateStamina(DeltaTime);
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::Jump);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(EKeyPressAction, ETriggerEvent::Started, this, &AMyCharacter::EKeyPress);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AMyCharacter::Dodge);
	}
}

void AMyCharacter::Jump()
{
	if (CharacterState == ECharacterState::ECS_Unequipped)
	{
		Super::Jump();
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Attributes->ReceiveDamage(DamageAmount);
	UpdateHealthBar();
	return DamageAmount;
}

void AMyCharacter::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Hitter);
	DisableWeapon();
	HandleHitReaction();
}

void AMyCharacter::HandleHitReaction()
{
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		CharacterAnimaionState = ECharacterAnimationState::EAS_HitReaction;
	}
}

void AMyCharacter::DisableWeapon()
{
	if (EquippedWeapon) EquippedWeapon->WeaponSetCollision(ECollisionEnabled::NoCollision);
}

void AMyCharacter::DisableMesh()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMyCharacter::CharacterDeadState()
{
	CharacterState = ECharacterState::ECS_Dead;
}

void AMyCharacter::SetItem(AItem* Item)
{
	MyItem = Item;
}

void AMyCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && MyOverlay)
	{
		Attributes->AddSouls(Soul->GetSoul());
		MyOverlay->SetSoul(Attributes->GetSoul());
	}
}

void AMyCharacter::AddGold(Atreasure* Gold)
{
	if (Attributes && MyOverlay)
	{
		Attributes->AddGold(Gold->GetGold());
		MyOverlay->SetGold(Attributes->GetGold());
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller && CharacterAnimaionState == ECharacterAnimationState::EAS_None)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyCharacter::EKeyPress()
{
	AWeapon* MyWeapon = Cast<AWeapon>(MyItem);
	if (MyWeapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		MyWeapon->ItemEquip(GetMesh(),FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquipOneHand;
		EquippedWeapon = MyWeapon;
		MyWeapon = nullptr;
	}
}

void AMyCharacter::Attack()
{
	if (CharacterState == ECharacterState::ECS_EquipOneHand &&CharacterAnimaionState != ECharacterAnimationState::EAS_HitReaction)
	{
		CharacterAnimaionState = ECharacterAnimationState::EAS_Attacking;
		PlayAttackMontage(AttackMontage, MontageSection);
	}
}

void AMyCharacter::Dodge()
{
	if (CharacterAnimaionState != ECharacterAnimationState::EAS_None) return;
	if (Attributes && Attributes->GetStamina() >Attributes->GetDodgeCost())
	{
		PlayMontage(FName("Dodge1"), DodgeMontage);
		CharacterAnimaionState = ECharacterAnimationState::EAS_Dodge;
		Attributes->UseStamina(Attributes->GetDodgeCost());
		if (MyOverlay)
		{
			MyOverlay->SetStaminaProgressBarPercent(Attributes->GetStaminaPercent());
		}
	}
}

void AMyCharacter::PlayAttackMontage(UAnimMontage* Montage , TArray<FName> Section)
{
	Super::PlayAttackMontage(Montage, Section);
}

void AMyCharacter::AttackEnd()
{
	CharacterAnimaionState = ECharacterAnimationState::EAS_None;
}

void AMyCharacter::DodgeEnd()
{
	CharacterAnimaionState = ECharacterAnimationState::EAS_None;
}

void AMyCharacter::HitReactEnd()
{
	CharacterAnimaionState = ECharacterAnimationState::EAS_None;
}

void AMyCharacter::AddDefaultMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyCharacter::AddEngageableTargetTag()
{
	Tags.Add(EngageableTargetTag);
}

void AMyCharacter::InitializeHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
		if (MyHUD)
		{
			MyOverlay = MyHUD->GetMyOverlay();
		}
	}
}

void AMyCharacter::InitializeMyOverlay()
{
	if (MyOverlay && Attributes)
	{
		MyOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		MyOverlay->SetSoul(0);
		MyOverlay->SetGold(0);
	}
}

void AMyCharacter::UpdateStamina(float DeltaTime)
{
	if (Attributes && MyOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		MyOverlay->SetStaminaProgressBarPercent(Attributes->GetStaminaPercent());
	}
}

void AMyCharacter::UpdateHealthBar()
{
	if (MyOverlay && Attributes)
	{
		MyOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}
