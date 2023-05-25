// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"


/**
 * 
 */
UCLASS()
class RPG_GAME_API AMyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AMyCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

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

	UPROPERTY(EditAnywhere, Category = Input)
	class UAnimMontage* AttackMontage;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPress();
	void Attack();
	void PlayAttackMontage();
	
	UPROPERTY(VisibleAnywhere,Category="Item")
	class AItem* MyItem;

	

	UPROPERTY(EditAnywhere,Category = "Montage")
	TArray<FName> MontageSection;

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void SetCollision(ECollisionEnabled::Type CollisionEnabled);



private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	ECharacterAnimationState CharacterAnimaionState = ECharacterAnimationState::EAS_None;
public:
	void SetItme(AItem* Item) { MyItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const{
		return CharacterState;
	}
};

