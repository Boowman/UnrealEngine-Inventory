// Fill out your copyright notice in the Description page of Project Settings.

#include "Item_Canteen.h"
#include "FPSInventoryCharacter.h"
#include "Engine.h"

void AItem_Canteen::EquipItem()
{
	Super::EquipItem();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("Use Object PickUp: ") + Name);

	if (ContentLeft > 0)
	{
		ContentLeft -= 25;

		if (ContentLeft < 0)
			ContentLeft = 0;

		Player->GiveWater(25);
	}
}