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

void AWeapon::ItemEquip(USceneComponent* Parent, AActor* NewOwner, APawn* NewInstigator)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true);
	Mesh->AttachToComponent(Parent, TransformRules, FName("RightHandSocket"));
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
}

void AWeapon::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	const FVector StartLoctaion = Start->GetComponentLocation();
	const FVector EndLocation = End->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActor) // 맞은 배우를 한번만 맞게해준다.
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		StartLoctaion,
		EndLocation,
		FVector(5.f, 5.f, 5.f),
		Start->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore, //무시할 액터
		EDrawDebugTrace::None,
		BoxHit,
		true //이 클래스를 무시할건지
	);

	if (BoxHit.GetActor())
	{
		IMyInterface* HitInterface = Cast<IMyInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}
		IgnoreActor.AddUnique(BoxHit.GetActor()); //맞은 배우를 배열에 저장해줌
		// 캐릭터에 클래스에 가서 배열을 다시 비워줘야 계속 타격 가능
		
		CreateFields(BoxHit.ImpactPoint);

		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);
	}

}


