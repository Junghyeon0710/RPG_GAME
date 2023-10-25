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
	void MoveSoul(float DeltaTime);
protected:
	//스페어에 닳을시
	virtual void BeginPlay() override;
	virtual void SphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	void HandleItemPickup(AActor* OtherActor);
private:
	void CalculateDesiredZOffset();
	
	// 아이템의 현재 영혼 수입니다.
	UPROPERTY(EditAnywhere)
	int32 Soul;

	// 아이템이 움직이는 데 사용하는 원하는 Z 위치입니다.
	UPROPERTY(EditAnywhere)
	double DesiredZ;

	// 아이템의 이동 속도입니다. 음수 값으로 설정하면 아이템이 아래로 내려가게 됩니다.
	UPROPERTY(EditAnywhere)
	float DriftRate = -15.f;

public:
	FORCEINLINE int32 GetSoul() const { return Soul; }
	FORCEINLINE void SetSoul(int32 NumberSouls) { Soul = NumberSouls; }
};
