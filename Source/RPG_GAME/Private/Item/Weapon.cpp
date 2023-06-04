// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon.h"
#include <Components/BoxComponent.h>
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "../Public/Interface/MyInterface.h"
#include <Debug/DebugDrawComponent.h>

AWeapon::AWeapon()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(GetRootComponent());

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Start = CreateDefaultSubobject<USceneComponent>(TEXT("Start"));
	Start->SetupAttachment(GetRootComponent());

	End = CreateDefaultSubobject<USceneComponent>(TEXT("End"));
	End->SetupAttachment(GetRootComponent());
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BoxBeginOverlap);
}

void AWeapon::ItemEquip(USceneComponent* Parent, const FName Name,AActor* NewOwner, APawn* NewInstigator)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true);
	Mesh->AttachToComponent(Parent, TransformRules, Name);
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
}

void AWeapon::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		return;//무기 소유한 사람도 적이고 상대도 적이라면 
	}

	const FVector StartLoctaion = Start->GetComponentLocation();
	const FVector EndLocation = End->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActor) // 맞은 배우를 한번만 맞게해준다.
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		StartLoctaion,
		EndLocation,
		BoxSize,
		Start->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore, //무시할 액터
		BoxDebug ? EDrawDebugTrace::ForDuration:EDrawDebugTrace::None,
		BoxHit,
		true //이 클래스를 무시할건지
	);
	IgnoreActor.AddUnique(BoxHit.GetActor()); //맞은 배우를 배열에 저장해줌
	// 캐릭터에 클래스에 가서 배열을 다시 비워줘야 계속 타격 가능
	DrawDebugSphere(GetWorld(), BoxHit.ImpactPoint, 12.f, 10, FColor::White, false, 3.f);

	if (BoxHit.GetActor())
	{
		if (GetOwner()->ActorHasTag(TEXT("Enemy")) && BoxHit.GetActor()->ActorHasTag(TEXT("Enemy")))
		{
			return;//무기 소유한 사람도 적이고 상대도 적이라면 
		}
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IMyInterface* HitInterface = Cast<IMyInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint,GetOwner());
		}
		
		CreateFields(BoxHit.ImpactPoint); //필드생성 액터 부시기

		
	}

}

void AWeapon::WeaponSetCollision(ECollisionEnabled::Type CollisionEnabled)
{
	Box->SetCollisionEnabled(CollisionEnabled);
}


