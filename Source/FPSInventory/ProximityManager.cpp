// Fill out your copyright notice in the Description page of Project Settings.

#include "ProximityManager.h"
#include "Item.h"
#include "Inventory.h"
#include "Engine.h"
#include "GUI/UI_Slots.h"
#include "FPSInventoryCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UProximityManager::UProximityManager()
{
	// ...
}

// Called when the game starts
void UProximityManager::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AFPSInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));	
}

void UProximityManager::AddItem(AItem * Item)
{
	if (Item != nullptr)
	{
		Items.Add(Item);

		for (int8 i = 0; i < Player->UIMenus->ProximityList.Num(); i++)
		{
			if (Player->UIMenus->ProximityList[i]->Visibility == ESlateVisibility::Collapsed)
			{
				Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->StoredItem = Item;
				Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->IsAvailable = false;
				Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->ContentLeftProgressBar();

				Player->UIMenus->ProximityList[i]->SetVisibility(ESlateVisibility::Visible);

				break;
			}
		}
	}
}

void UProximityManager::AddInventory(AInventory * Inventory)
{
	if (Inventory != nullptr)
	{
		Inventories.Add(Inventory);

		for (int8 i = 0; i < Player->UIMenus->ProximityList.Num(); i++)
		{
			if (Player->UIMenus->ProximityList[i]->Visibility == ESlateVisibility::Collapsed)
			{
				Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->StoredItem = Inventory;
				Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->StoredInventory = Inventory;
				Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->IsAvailable = false;
				Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->ContentLeftProgressBar();

				Player->UIMenus->ProximityList[i]->SetVisibility(ESlateVisibility::Visible);

				break;
			}
		}
	}
}

void UProximityManager::RemoveItem(AItem * Item)
{
	if (Item != nullptr)
	{
		for (int8 i = 0; i < Player->UIMenus->ProximityList.Num(); i++)
		{
			if (Player->UIMenus->ProximityList[i]->Visibility == ESlateVisibility::Visible)
			{
				if (Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->StoredItem->ID == Item->ID)
				{
					Player->UIMenus->ProximityList[i]->SetVisibility(ESlateVisibility::Collapsed);

					Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->StoredItem = nullptr;
					Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->IsAvailable = true;
					Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->ContentLeftProgressBar();

					break;
				}
			}
		}

		Items.Remove(Item);
	}
}

void UProximityManager::RemoveInventory(AInventory * Inventory)
{
	if (Inventory != nullptr)
	{
		for (int8 i = 0; i < Player->UIMenus->ProximityList.Num(); i++)
		{
			if (Player->UIMenus->ProximityList[i]->Visibility == ESlateVisibility::Visible)
			{
				if (Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->StoredItem->ID == Inventory->ID)
				{
					Player->UIMenus->ProximityList[i]->SetVisibility(ESlateVisibility::Collapsed);

					Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->StoredItem = nullptr;
					Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->StoredInventory = nullptr;
					Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->IsAvailable = true;
					Cast<UUI_Slots>(Player->UIMenus->ProximityList[i])->ContentLeftProgressBar();

					break;
				}
			}
		}

		Inventories.Remove(Inventory);
	}
}