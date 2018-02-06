// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_SlotQuickMenu.h"
#include "PickupItem.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FPSInventoryCharacter.h"
#include "Engine.h"
#include "Item.h"
#include "GUI/UI_Slots.h"

void UUI_SlotQuickMenu::SetupQuickMenu(AItem* Item, UUI_Slots* UI_Slot)
{
	if(Item != nullptr)
	{		
		this->SetVisibility(ESlateVisibility::Visible);
		this->Item = Item;
		this->UI_Slot = UI_Slot;
	}
}

void UUI_SlotQuickMenu::DropObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("DropObject"));

	if (UI_Slot->StoredItem != nullptr)
	{
		if (UI_Slot->IsStorred)
		{
			//Only remove items if there are more than 1
			UI_Slot->StoredInventory->DropItem(UI_Slot->StoredItems[0], false);
			UI_Slot->StoredItems.RemoveAt(0);

			UI_Slot->CurrentQuantity--;
			UI_Slot->UpdateQuantityPanel();

			if (UI_Slot->StoredItems.Num() < 1)
			{
				this->SetVisibility(ESlateVisibility::Hidden);
				UI_Slot->ResetSlot();
			}
		}
	}
}

void UUI_SlotQuickMenu::UseObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Use Object"));

	if (UI_Slot->StoredItem != nullptr)
	{
		Item->EquipItem();

		if (Cast<APickupItem>(Item)->Reusable != true)
		{
			UI_Slot->StoredInventory->TakeWeight(UI_Slot->StoredItem->Weight);

			for (int8 i = 0; i < UI_Slot->StoredItems.Num(); i++)
			{
				UI_Slot->StoredInventory->ItemsStored.Remove(UI_Slot->StoredItems[i]);
				UI_Slot->StoredItems.RemoveAt(i);
				break;
			}

			UI_Slot->CurrentQuantity--;
			UI_Slot->UpdateQuantityPanel();

			if (UI_Slot->StoredItems.Num() < 1)
			{
				this->SetVisibility(ESlateVisibility::Hidden);
				UI_Slot->ResetSlot();
			}
		}
		else
			UI_Slot->ContentLeftProgressBar();
	}
}