// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "InputActionValue.h"
#include <Interface/PickupInterface.h>
#include "MyCharacter.generated.h"


/**
 * 
 */
UCLASS()
class RPG_GAME_API AMyCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()
public:
	AMyCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void SetItem(AItem* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;
	virtual void AddGold(class Atreasure* Gold) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;
	void HandleHitReaction();
	void DisableWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void AttackEnd() override;
	virtual void PlayAttackMontage(UAnimMontage* Montage, TArray<FName> Section) override;
	void DisableMesh();
	void CharacterDeadState();
	void Move(const FInputActionValue& Value); //캐릭터 움직이
	void Look(const FInputActionValue& Value); //캐릭터 마우스로 보기
	void EKeyPress(); // E키를 누르면
	void Attack(); // 왼쪽마우스를 누르면
	void Dodge(); //shift키 누르면

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UPROPERTY(VisibleAnywhere,Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category =Camera)
	class UCameraComponent* Camera;

	/** 기본매핑콘텍스트 */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	/** 점프 입력 액션 */
	UPROPERTY(EditAnywhere,Category=Input)
	class UInputAction* JumpAction;

	/** 움직임 입력 액션 */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MoveAction;

	/** 화면 입력 액션 */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* LookAction;

	/** 장착 입력 액션 */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* EKeyPressAction;

	/** 공격 입력 액션 */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* AttackAction;

	/** 회피 입력 액션 */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* DodgeAction;

	UPROPERTY(VisibleAnywhere,Category="Item")
	class AItem* MyItem;

private:
	void AddDefaultMappingContext();
	void AddEngageableTargetTag();
	void InitializeHUD();
	void InitializeMyOverlay();
	void UpdateStamina(float DeltaTime);
	void UpdateHealthBar();

	UPROPERTY()
	class UMainCharacterOverlay* MyOverlay;

	UPROPERTY(EditAnywhere, Category=  Montage)
	class UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere)
	FName EngageableTargetTag = "EngagebleTarget";

public:
	FORCEINLINE ECharacterState GetCharacterState() const{return CharacterState;	}
};

