// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class RPG_GAME_API ASoul : public AItem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime)override;
protected:
	//스페어에 닳을시
	virtual void BeginPlay() override;
	virtual void SphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
private:
	UPROPERTY(EditAnywhere)
	int32 Soul;

	UPROPERTY(EditAnywhere)
	double DesiredZ;

	UPROPERTY(EditAnywhere)
	float DriftRate = -15.f;
public:
	FORCEINLINE int32 GetSoul() const { return Soul; }
	FORCEINLINE void SetSoul(int32 NumberSouls) { Soul = NumberSouls; }
};
