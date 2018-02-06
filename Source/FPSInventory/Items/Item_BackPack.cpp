// Fill out your copyright notice in the Description page of Project Settings.

#include "Item_BackPack.h"
#include "FPSInventoryCharacter.h"
#include "Engine.h"

void AItem_BackPack::EquipItem()
{
	Super::EquipItem();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("Use Object PickUp: ") + Name);
}

