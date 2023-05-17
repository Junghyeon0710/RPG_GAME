// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class RPG_GAME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	//스페어에 닳을시
	UFUNCTION()
	virtual void SphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//스페어 콜리저에서 떨어질시
	UFUNCTION()
	virtual void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:

	UPROPERTY(EditAnywhere,Category = "사인")
	float TimeConstant = 5.f; // 

	UPROPERTY(EditAnywhere, Category = "사인")
	float Amplitude = .25f;  //진폭 

	UPROPERTY(VisibleAnywhere, Category = "사인")
	float RunningTime; //시간

	float MySin(); // 사인구하기


};
