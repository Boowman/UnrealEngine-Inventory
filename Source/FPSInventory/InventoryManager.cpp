// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManager.h"
#include "Item.h"
#include "Engine.h"
#include "FPSInventoryCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "GUI/UI_Inventory.h"
#include "GUI/UI_Slots.h"

// Sets default values for this component's properties
UInventoryManager::UInventoryManager()
{

}

// Called when the game starts
void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AFPSInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

TArray<AInventory*> UInventoryManager::GetInventoryList()
{
	return InventoryList;
}

void UInventoryManager::AddInventory(AInventory* Inventory)
{
	if (Inventory != nullptr)
	{
		InventoryList.Add(Inventory);

		for (int8 i = 0; i < Player->UIMenus->InventoryList.Num(); i++)
		{
			if (Player->UIMenus->InventoryList[i]->Visibility == ESlateVisibility::Collapsed)
			{
				UUI_Inventory* UI_Inventory = Cast<UUI_Inventory>(Player->UIMenus->InventoryList[i]);

				ECollisionEnabled::Type collision = ECollisionEnabled::NoCollision;

				Inventory->Mesh->SetVisibility(false);
				Inventory->Mesh->SetSimulatePhysics(false);
				Inventory->Mesh->SetCollisionEnabled(collision);

				Inventory->Collider->SetCollisionEnabled(collision);
				Inventory->EquipItem();

				Inventory->UI_Inventory = UI_Inventory;

				UI_Inventory->Inventory = Inventory;
				UI_Inventory->MaxSlots  = Inventory->MaxSlots;

				UI_Inventory->ShowAvailableSlots();
				UI_Inventory->AssignInventoryToSlots();

				for (int8 j = 0; j < UI_Inventory->Inventory->ItemsStored.Num(); j++)
				{
					for (int8 x = 0; x < UI_Inventory->InventorySlots.Num(); x++)
					{
						UUI_Slots* UI_Slot = Cast<UUI_Slots>(Cast<UUI_Inventory>(Player->UIMenus->InventoryList[i])->InventorySlots[x]);

						if (UI_Slot->Visibility == ESlateVisibility::Visible)
						{
							if (UI_Slot->SlotID == UI_Inventory->Inventory->ItemsStored[j]->SlotID)
							{
								if (UI_Slot->IsAvailable)
								{
									UI_Slot->UpdateSlot(UI_Inventory->Inventory->ItemsStored[j]);
									break;
								}
							}
						}
					}
				}

				
				Player->UIMenus->InventoryList[i]->SetVisibility(ESlateVisibility::Visible);
				Player->AddWeight(Inventory->Weight);

				break;
			}
		}
	}
}

void UInventoryManager::DropInventory(AInventory* Inventory)
{
	for (int8 i = 0; i < InventoryList.Num(); i++)
	{
		if (InventoryList[i]->ID == Inventory->ID)
		{
			ECollisionEnabled::Type collision = ECollisionEnabled::QueryAndPhysics;

			Inventory->Mesh->SetVisibility(true);
			Inventory->Mesh->SetSimulatePhysics(true);
			Inventory->Mesh->SetCollisionEnabled(collision);

			//Spawn the object in front of the player
			Inventory->SetActorLocation(Player->GetActorLocation() + (Player->GetActorForwardVector() * 150));

			Inventory->Collider->SetCollisionEnabled(collision);

			InventoryList.Remove(Inventory);

			break;
		}
	}

	for (int8 i = 0; i < Player->UIMenus->InventoryList.Num(); i++)
	{
		if (Player->UIMenus->InventoryList[i]->Visibility == ESlateVisibility::Visible)
		{
			if (Cast<UUI_Inventory>(Player->UIMenus->InventoryList[i])->Inventory->ID == Inventory->ID)
			{
				Player->UIMenus->ProximityList[i]->SetVisibility(ESlateVisibility::Collapsed);

				Cast<UUI_Inventory>(Player->UIMenus->InventoryList[i])->Inventory = nullptr;
				Cast<UUI_Inventory>(Player->UIMenus->InventoryList[i])->MaxSlots = 0;

				break;
			}
		}
	}
}

///
/// Assign a random ID to the specific item
/// Also make sure the item ID doesn't exist already.
///
void UInventoryManager::AssignAnItemID(AItem * Item)
{
	float tempNum = FMath::RandRange(0, 1000);

	if (ItemsID.Num() == 0)
	{
		ItemsID.Add(tempNum);
		Item->ID = FMath::RoundToInt(tempNum);
	}
	else
	{
		while (ItemsID.Contains(tempNum))
		{
			tempNum = FMath::RandRange(0, 1000);
		}

		if (!ItemsID.Contains(tempNum))
		{
			ItemsID.Add(tempNum);
			Item->ID = FMath::RoundToInt(tempNum);
		}
	}
}