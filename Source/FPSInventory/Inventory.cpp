// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "InventoryManager.h"
#include "ProximityManager.h"
#include "Engine.h"
#include "GUI/UI_Slots.h"
#include "GUI/UI_Inventory.h"
#include "FPSInventoryCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
AInventory::AInventory()
{	
	// ...
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();

	//Try to find a solution and maybe call it in the Constructor
	Collider->OnComponentBeginOverlap.RemoveDynamic(this, &AInventory::OnOverlapBegin);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AInventory::OnOverlapBegin);

	//Try to find a solution and maybe call it in the Constructor
	Collider->OnComponentEndOverlap.RemoveDynamic(this, &AInventory::OnOverlapEnd);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AInventory::OnOverlapEnd);
}

void AInventory::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->ActorHasTag(FName("Player")))
		{
			if (Player != nullptr)
			{
				if (ProximityManager != nullptr)
					ProximityManager->AddInventory(this);
			}
		}
	}
}

void AInventory::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->ActorHasTag(FName("Player")))
		{
			if (Player != nullptr)
			{
				if (ProximityManager != nullptr)
					ProximityManager->RemoveInventory(this);
			}
		}
	}
}

void AInventory::PickUpItem(AItem* PickedupItem, bool bDragDrop, bool bUpdateUsedSlot)
{
	if (PickedupItem != nullptr)
	{
		if (UsedSlots < MaxSlots)
		{
			//Add item to inventory
			ItemsStored.Add(PickedupItem);
			
			if(bUpdateUsedSlot)
				UsedSlots += 1;

			ECollisionEnabled::Type collision = ECollisionEnabled::NoCollision;

			PickedupItem->Mesh->SetVisibility(false);
			PickedupItem->Mesh->SetSimulatePhysics(false);
			PickedupItem->Mesh->SetCollisionEnabled(collision);

			PickedupItem->Collider->SetCollisionEnabled(collision);

			if (bDragDrop == false)
			{
				for (int8 i = 0; i < MaxSlots; i++)
				{
					if (UI_Inventory->InventorySlots[i]->Visibility == ESlateVisibility::Visible)
					{
						UUI_Slots* TempSlot = Cast<UUI_Slots>(UI_Inventory->InventorySlots[i]);

						if (TempSlot->IsAvailable == true)
						{
							TempSlot->StoredItem = PickedupItem;
							TempSlot->StoredItem->SlotID = i;

							TempSlot->IsAvailable = false;
							TempSlot->IsDraggable = true;
							TempSlot->IsStorred = true;

							break;
						}
					}
				}
			}	
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("No More Space Available"));

			//Add item to inventory
			ItemsStored.Add(PickedupItem);

			ECollisionEnabled::Type collision = ECollisionEnabled::NoCollision;

			PickedupItem->Mesh->SetVisibility(false);
			PickedupItem->Mesh->SetSimulatePhysics(false);
			PickedupItem->Mesh->SetCollisionEnabled(collision);

			PickedupItem->Collider->SetCollisionEnabled(collision);
		}

		Player->AddWeight(PickedupItem->Weight);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("PickedUpItem is null"));
}

void AInventory::DropItem(AItem* StoredItem, bool bUpdateUsedSlot)
{
	for (int8 i = 0; i < ItemsStored.Num(); i++)
	{
		if (ItemsStored[i]->ID == StoredItem->ID)
		{		
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::FromInt(ItemsStored[i]->ID) + FString("-") + FString::FromInt(StoredItem->ID));

			CurrentWeight -= StoredItem->Weight;

			if(bUpdateUsedSlot)
				UsedSlots -= 1;

			//Displaying the item in the world
			ECollisionEnabled::Type collision = ECollisionEnabled::QueryAndPhysics;

			ItemsStored[i]->Mesh->SetVisibility(true);
			ItemsStored[i]->Mesh->SetSimulatePhysics(true);
			ItemsStored[i]->Mesh->SetCollisionEnabled(collision);

			ItemsStored[i]->Collider->SetCollisionEnabled(collision);
			
			//Spawn the object in front of the player
			ItemsStored[i]->SetActorLocation(Player->GetActorLocation() + (Player->GetActorForwardVector() * 150));

			//Remove item to inventory
			ItemsStored.Remove(StoredItem);

			break;

			Player->TakeWeight(StoredItem->Weight);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("PickedUpItem is null"));
	}
}

bool AInventory::CheckIfEnoughStorage(TArray<AItem*> Items, AItem* Item)
{
	UE_LOG(LogTemp, Warning, TEXT("Check"));

	if (UsedSlots < MaxSlots)
	{
		UE_LOG(LogTemp, Warning, TEXT("Still Have Slots"));

		if (Items.Num() > 0 && CurrentWeight + (Items.Num() * Item->Weight) <= MaxWeight || Item != nullptr && (CurrentWeight + Item->Weight) <= MaxWeight)
		{
			UE_LOG(LogTemp, Warning, TEXT("Enough Weight"));
			return true;
		}
	}

	return false;
}

void AInventory::AddWeight(int32 value)
{
	CurrentWeight += value;
}

void AInventory::TakeWeight(int32 value)
{
	CurrentWeight -= value;
}