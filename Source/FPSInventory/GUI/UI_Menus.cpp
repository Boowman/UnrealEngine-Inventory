// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Menus.h"
#include "Components/WrapBox.h"
#include "FPSInventoryCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GUI/UI_Slots.h"
#include "GUI/UI_Inventory.h"
#include "Engine.h"

float UUI_Menus::GetCurrentTime()
{
	return 0.0f;
}

void UUI_Menus::InitializeUIMenus(UWrapBox * ProximityListParent, UWrapBox * InventoryListParent, UUserWidget* DescriptionWidget, UUserWidget* DragDropWidget, UUserWidget* QuickMenuWidget)
{
	if(ProximityListParent != nullptr && InventoryListParent != nullptr && DescriptionWidget != nullptr && DragDropWidget != nullptr && QuickMenuWidget != nullptr)
	{		
		for (int8 i = 0; i < ProximityListParent->GetChildrenCount(); i++)
		{
			ProximityList.Add(Cast<UUserWidget>(ProximityListParent->GetChildAt(i)));
		}

		for (int8 i = 0; i < InventoryListParent->GetChildrenCount(); i++)
		{
			InventoryList.Add(Cast<UUserWidget>(InventoryListParent->GetChildAt(i)));

			for (int j = 0; j < Cast<UUI_Inventory>(InventoryList[i])->InventorySlots.Num(); j++)
			{
				Cast<UUI_Slots>(Cast<UUI_Inventory>(InventoryList[i])->InventorySlots[j])->InitializeSlot(DescriptionWidget, DragDropWidget, QuickMenuWidget);
			}
		}


		Character = Cast<AFPSInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

		for (int8 i = 0; i < ProximityList.Num(); i++)
		{
			Cast<UUI_Slots>(ProximityList[i])->InitializeSlot(DescriptionWidget, DragDropWidget, QuickMenuWidget);
		}
	}
}