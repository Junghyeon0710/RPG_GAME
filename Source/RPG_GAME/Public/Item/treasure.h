// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Item/Item.h>
#include "treasure.generated.h"

UCLASS()
class RPG_GAME_API Atreasure : public AItem
{
	GENERATED_BODY()
	
public:	
	Atreasure();

protected:
	virtual void BeginPlay() override;
	virtual void SphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	int32 Gold;
public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
