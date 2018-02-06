// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "Engine.h"
#include "InventoryManager.h"
#include "ProximityManager.h"
#include "Components/SphereComponent.h"

// Sets default values
APickupItem::APickupItem()
{
	// ...
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	//Try to find a solution and maybe call it in the Constructor
	Collider->OnComponentBeginOverlap.RemoveDynamic(this, &APickupItem::OnOverlapBegin);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnOverlapBegin);

	//Try to find a solution and maybe call it in the Constructor
	Collider->OnComponentEndOverlap.RemoveDynamic(this, &APickupItem::OnOverlapEnd);
	Collider->OnComponentEndOverlap.AddDynamic(this, &APickupItem::OnOverlapEnd);
}

void APickupItem::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->ActorHasTag(FName("Player")))
		{
			if (ProximityManager != nullptr && !ProximityManager->GetItems().Contains(this))
				ProximityManager->AddItem(this);
		}
	}
}

void APickupItem::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->ActorHasTag(FName("Player")))
		{
			if (ProximityManager != nullptr && ProximityManager->GetItems().Contains(this))
				ProximityManager->RemoveItem(this);
		}
	}
}