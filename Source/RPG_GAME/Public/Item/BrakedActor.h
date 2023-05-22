// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/MyInterface.h"
#include "BrakedActor.generated.h"


UCLASS()
class RPG_GAME_API ABrakedActor : public AActor,public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrakedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class Atreasure>> TreasureClass;
	bool bBarekd = false;

	UPROPERTY(EditAnywhere)
	class USoundBase* HitSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit(const FVector& ImpactPoint)override;
};
