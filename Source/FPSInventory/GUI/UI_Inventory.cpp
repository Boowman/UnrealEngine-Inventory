// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Inventory.h"
#include "InventoryManager.h"
#include "Item.h"
#include "Engine.h"
#include "GUI/UI_Slots.h"
#include "Math/UnrealMathUtility.h"
#include "Components/WrapBox.h"
#include "Components/Widget.h"
#include "Components/SizeBox.h"
#include "Components/StaticMeshComponent.h"

void UUI_Inventory::InitializeInventory(UWrapBox* SlotsListParent, USizeBox* SizeBoxWidget)
{
	if(SlotsListParent != nullptr)
	{	
		for (int8 i = 0; i < SlotsListParent->GetChildrenCount(); i++)
		{
			InventorySlots.Add(Cast<UUserWidget>(SlotsListParent->GetChildAt(i)));
		}

		this->SizeBox = SizeBoxWidget;
	}
}

void UUI_Inventory::ShowAvailableSlots()
{
	for (int8 i = (InventorySlots.Num() - 1); i >= MaxSlots; --i)
	{
		if(InventorySlots[i]->Visibility == ESlateVisibility::Visible)
			InventorySlots[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//Set the variable to the value following value 
	float heightBasedOnSlots = MaxSlots / 5;;
	
	//Just making just it will always never be less than 1
	if (heightBasedOnSlots < 0.99)
		heightBasedOnSlots = 1;

	//Set the Height of the SizeBox to the following
	//RoundFromZero it will always round up the number 1.1 -> 2, than multiply to the height of the slot which is 110
	SizeBox->SetHeightOverride(100 + FMath::RoundFromZero(heightBasedOnSlots) * 110);
}

void UUI_Inventory::AssignInventoryToSlots()
{
	for (int8 i = 0; i < InventorySlots.Num(); i++)
	{
		Cast<UUI_Slots>(InventorySlots[i])->StoredInventory = Inventory;
	}
}

void UUI_Inventory::DropInventory()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString("DropInventory"));

	if (Inventory != nullptr)
	{
		//Drop the invetory object
		Inventory->InventoryManager->DropInventory(Inventory);

		//Set all the slots to be visible
		for (int8 i = 0; i < InventorySlots.Num(); i++)
		{
			InventorySlots[i]->SetVisibility(ESlateVisibility::Visible);
			Cast<UUI_Slots>(InventorySlots[i])->ResetSlot();
		}

		//Hide this UI element
		this->SetVisibility(ESlateVisibility::Collapsed);

		//Set the inventory to null
		Inventory = nullptr;
	}
}

FText UUI_Inventory::DisplayWeight()
{
	if (Inventory != nullptr)
	{
		FString tempString = FString::FromInt(Inventory->CurrentWeight) + "/" + FString::FromInt(Inventory->MaxWeight) + " Kg";

		return FText::FromString(tempString);
	}
	else
		return FText::FromString("");
}
