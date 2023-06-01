// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_GAME_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;

	void ItemEquip(USceneComponent* Parent,const FName Name,AActor* NewOwner , APawn* NewInstigator);

	TArray<AActor*> IgnoreActor;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void BoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Start;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* End;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
 
private:

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;
public:
	FORCEINLINE UBoxComponent* GetBox() const { return Box; }
	void WeaponSetCollision(ECollisionEnabled::Type CollisionEnabled);
};
