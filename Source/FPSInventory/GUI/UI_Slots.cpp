// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Slots.h"
#include "PickupItem.h"
#include "FPSInventoryCharacter.h"
#include "InventoryManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Engine/GameEngine.h"
#include "GUI/UI_Inventory.h"
#include "GUI/UI_SlotQuickMenu.h"

void UUI_Slots::InitializeSlot(UUserWidget * DescriptionWidget, UUserWidget * DragDropWidget, UUserWidget * QuickMenuWidget)
{
	this->DescriptionBox = Cast<UUI_Slots>(DescriptionWidget);
	this->DragDropSlot = Cast<UUI_Slots>(DragDropWidget);
	this->QuickMenu = Cast<UUI_SlotQuickMenu>(QuickMenuWidget);
}

//------------------------------------------------------
/*If we need more details or change something regarding the slot*/

void UUI_Slots::ShowDragDropSlot(UUI_Slots * Widget, bool bVisibility)
{

}

void UUI_Slots::ShowDescription(UUI_Slots* Widget, bool bVisibility)
{
	if (Widget != nullptr)
	{
		this->DescriptionBox = Widget;

		if (bVisibility)
		{
			Widget->StoredItem = this->StoredItem;
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
			Widget->StoredItem = nullptr;
		}
	}
}

void UUI_Slots::ShowQuickMenu(AItem* Item, UUI_Slots* UI_Slot)
{
	if(Item != nullptr && UI_Slot != nullptr)
		QuickMenu->SetupQuickMenu(Item, UI_Slot);
}

void UUI_Slots::DropOperation(UUI_Slots* UI_Slot)
{
	if(UI_Slot != nullptr)
	{		
		if (SlotID != 255 || UI_Slot->SlotID != 255)
		{
			if (UI_Slot->StoredItem != nullptr && StoredInventory != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, UI_Slot->StoredItem->Name);

				if (UI_Slot->IsStorred == false && IsStorred == false || UI_Slot->IsStorred == false && StoredItem != nullptr && UI_Slot->StoredItem->Name == StoredItem->Name)
					AddNewItem(UI_Slot);
				else if (UI_Slot->IsStorred == true && StoredItem != nullptr && UI_Slot->StoredItem->Name == StoredItem->Name)
					MergeSlots(UI_Slot);
				else if (UI_Slot->IsStorred == true && StoredItem == nullptr)
					MoveSlot(UI_Slot);
				else if (UI_Slot->IsStorred == false && IsStorred == true)
					ReplaceItem(UI_Slot);
				else if (UI_Slot->IsStorred == true && IsStorred == true)
					SwapItem(UI_Slot);
			}

			ContentLeftProgressBar();
		}
	}
}

//------------------------------------------------------
/*Managing the item that goes in the slot or out of it*/

//Need to be reworked, used when clicking on an item in the proximity menu
void UUI_Slots::SelectItem()
{
	UE_LOG(LogTemp, Warning, TEXT("SelectItem"));

	if (StoredItem != nullptr)
	{
		if (IsStorred == false && StoredItem->ItemType == EItemType::TYPE_Inventory)
			this->StoredInventory->InventoryManager->AddInventory(StoredInventory);
		else if (IsStorred == true && StoredItem != nullptr && StoredItems.Num() != 0)
			this->DropCurrentItem();
	}
}

void UUI_Slots::AddNewItem(UUI_Slots* UI_Slot)
{
	if (StoredInventory->CheckIfEnoughStorage(UI_Slot->StoredItems, UI_Slot->StoredItem))
	{
		if (CurrentQuantity < MaximumQuantity || MaximumQuantity == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Add new items"));

			CurrentQuantity++;

			bool dontUpdateUsedSlots = !StoredItem->IsValidLowLevel();

			UpdateSlot(UI_Slot->StoredItem);

			StoredItems.Add(UI_Slot->StoredItem);

			StoredInventory->PickUpItem(UI_Slot->StoredItem, true, dontUpdateUsedSlots);

			if(UI_Slot->StoredInventory == nullptr)
				StoredInventory->AddWeight(StoredItem->Weight);
			else if (UI_Slot->StoredInventory != nullptr && StoredInventory->Name != UI_Slot->StoredInventory->Name)
				StoredInventory->AddWeight(UI_Slot->StoredItems.Num() * UI_Slot->StoredItem->Weight);
		}
	}
}

void UUI_Slots::MoveSlot(UUI_Slots* UI_Slot)
{
	if ((StoredInventory->CurrentWeight + UI_Slot->StoredItem->Weight) <= StoredInventory->MaxWeight)
	{
		if (SlotID != UI_Slot->SlotID || StoredInventory->Name != UI_Slot->StoredInventory->Name)
		{
			if (StoredItem == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Copy over the array"));

				StoredItems = UI_Slot->StoredItems; //Copy over the array
				CurrentQuantity = UI_Slot->CurrentQuantity;

				UpdateSlot(UI_Slot->StoredItem);

				if (StoredItem != nullptr && StoredInventory->Name != UI_Slot->StoredInventory->Name)
				{
					for (int8 i = 0; i < UI_Slot->StoredItems.Num(); i++)
					{
						StoredInventory->ItemsStored.Add(StoredItems[i]);

						for (int8 j = 0; j < UI_Slot->StoredInventory->ItemsStored.Num(); j++)
						{
							if (UI_Slot->StoredInventory->ItemsStored[j]->ID == UI_Slot->StoredItems[i]->ID)
							{
								UI_Slot->StoredInventory->ItemsStored.Remove(UI_Slot->StoredItems[i]);
							}
						}
					}

					StoredInventory->AddWeight(StoredItems.Num() * StoredItem->Weight);
					UI_Slot->StoredInventory->TakeWeight(UI_Slot->StoredItems.Num() * UI_Slot->StoredItem->Weight);
				}

				UI_Slot->ResetSlot();
			}
		}
	}

	/*Need to swap the items if they are both in the same slot*/
}

void UUI_Slots::MergeSlots(UUI_Slots* UI_Slot)
{
	if ((StoredInventory->CurrentWeight + UI_Slot->StoredItem->Weight) <= StoredInventory->MaxWeight)
	{
		if (SlotID != UI_Slot->SlotID || StoredInventory->Name != UI_Slot->StoredInventory->Name)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Merge items"));

			for (int8 i = (UI_Slot->StoredItems.Num() - 1); i >= 0; i--)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(CurrentQuantity) + " - " + FString::FromInt(MaximumQuantity) + " - " + FString::FromInt(i) + " - " + FString::FromInt(UI_Slot->StoredItems.Num()));

				if (CurrentQuantity < MaximumQuantity)
				{
					CurrentQuantity++;
					UI_Slot->CurrentQuantity--;

					StoredItems.Add(UI_Slot->StoredItems[i]);

					if (StoredInventory->Name != UI_Slot->StoredInventory->Name)
					{
						for (int8 j = 0; j < UI_Slot->StoredInventory->ItemsStored.Num(); j++)
						{
							if (UI_Slot->StoredInventory->ItemsStored[j]->ID == UI_Slot->StoredItems[i]->ID)
							{
								StoredInventory->ItemsStored.Add(UI_Slot->StoredItems[i]);
								UI_Slot->StoredInventory->ItemsStored.Remove(UI_Slot->StoredItems[i]);
							}
						}

						StoredInventory->AddWeight(StoredItem->Weight);
						UI_Slot->StoredInventory->TakeWeight(UI_Slot->StoredItem->Weight);
					}

					UI_Slot->StoredItems.RemoveAt(i);
				}
				else
					break;
			}



			if (UI_Slot->StoredItems.Num() == 0)
				UI_Slot->ResetSlot();

			UpdateQuantityPanel();
			UI_Slot->UpdateQuantityPanel();
		}
	}
}

void UUI_Slots::ReplaceItem(UUI_Slots* UI_Slot)
{
	if (StoredItem != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Drop old items and place new items"));

		if (StoredItem->Name != UI_Slot->StoredItem->Name)
		{
			for (int8 i = 0; i < StoredItems.Num(); i++)
			{
				StoredInventory->DropItem(StoredItems[i], false);
			}

			ResetSlot();
			AddNewItem(UI_Slot);
		}
	}
}

void UUI_Slots::DropCurrentItem()
{
	//Only remove items if there are more than 1
	StoredInventory->DropItem(StoredItems[0], false);
	StoredItems.RemoveAt(0);

	CurrentQuantity--;
	UpdateQuantityPanel();

	if (StoredItems.Num() < 1)
		ResetSlot();
}

void UUI_Slots::UseCurrentItem()
{
	StoredItem->EquipItem();

	if (Cast<APickupItem>(StoredItem)->Reusable != true)
	{
		StoredInventory->TakeWeight(StoredItem->Weight);

		for (int8 i = 0; i < StoredItems.Num(); i++)
		{
			StoredInventory->ItemsStored.Remove(StoredItems[i]);
			StoredItems.RemoveAt(i);
			break;
		}

		CurrentQuantity--;
		UpdateQuantityPanel();

		if (StoredItems.Num() < 1)
			this->ResetSlot();
	}
	else
		ContentLeftProgressBar();
}

void UUI_Slots::SwapItem(UUI_Slots* UI_Slot)
{
	if (UI_Slot->StoredItem != nullptr && StoredItem != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Swapping Slots"));

		AItem* TempItem					= this->StoredItem;
		AInventory* TempInventory		= this->StoredInventory;
		TArray<AItem*> TempItems		= this->StoredItems;
		uint8 TempCurrentQuantity		= CurrentQuantity;

		this->SwapSlotUpdate(UI_Slot->StoredItem, UI_Slot->StoredInventory, UI_Slot->StoredItems, UI_Slot->CurrentQuantity);

		UI_Slot->SwapSlotUpdate(TempItem, TempInventory, TempItems, TempCurrentQuantity);
	}
}

void UUI_Slots::SwapSlotUpdate(AItem* TempItem, AInventory* TempInventory, TArray<AItem*> TempItems, uint8 TempCurrentQuantity)
{
	if (TempItem != nullptr && TempInventory != nullptr)
	{
		this->StoredItem = TempItem;
		
		for (int8 i = 0; i < this->StoredInventory->ItemsStored.Num(); i++)
		{
			for (int8 j = 0; j < this->StoredItems.Num(); j++)
			{
				if (this->StoredInventory->ItemsStored[i]->ID == this->StoredItems[j]->ID)
				{
					this->StoredInventory->ItemsStored.Remove(this->StoredItems[j]);
					TempInventory->ItemsStored.Add(this->StoredItems[j]);
				}
			}
		}

		this->CurrentQuantity = TempCurrentQuantity;
		this->StoredItems = TempItems;

		this->MaximumQuantity = Cast<APickupItem>(TempItem)->MaxQuantity;

		this->UpdateQuantityPanel();
		this->ContentLeftProgressBar();
	}
}

//------------------------------------------------------
/*Managing the slots status and look*/

void UUI_Slots::ResetSlot()
{
	if (StoredInventory != nullptr)
		StoredInventory->UsedSlots--;

	StoredItem->SlotID = 0;
	StoredItem = nullptr;

	IsAvailable		= true;
	IsDraggable		= true;
	IsStorred		= false;

	StoredItems.Empty();
	MaximumQuantity = 0;
	CurrentQuantity = 0;

	QuantityBackground->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Slots::UpdateSlot(AItem* Item)
{
	if(Item != nullptr)
		this->StoredItem = Item;

	this->IsAvailable	= false;
	this->IsStorred		= true;
	this->IsDraggable	= true;

	if(Cast<APickupItem>(Item) != nullptr)
		this->MaximumQuantity = Cast<APickupItem>(Item)->MaxQuantity;

	UpdateQuantityPanel();
}

void UUI_Slots::UpdateQuantityPanel()
{
	if (CurrentQuantity <= 1)
		this->QuantityBackground->SetVisibility(ESlateVisibility::Hidden);
	else
		this->QuantityBackground->SetVisibility(ESlateVisibility::Visible);
}

void UUI_Slots::ContentLeftProgressBar()
{
	if (ContentLeftProgress != nullptr)
	{
		if (StoredItem != nullptr)
		{
			APickupItem* Item = Cast<APickupItem>(StoredItem);

			if (Item->IsValidLowLevel())
			{
				if (Item->ContentLeft > 0)
				{
					ContentLeftProgress->SetPercent(Item->ContentLeft / 100);
					ContentLeftProgress->SetVisibility(ESlateVisibility::Visible);
				}
				else
					ContentLeftProgress->SetVisibility(ESlateVisibility::Hidden);
			}
			else
				ContentLeftProgress->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
