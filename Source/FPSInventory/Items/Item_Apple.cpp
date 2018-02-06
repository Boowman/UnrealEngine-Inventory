// Fill out your copyright notice in the Description page of Project Settings.

#include "Item_Apple.h"
#include "FPSInventoryCharacter.h"
#include "Engine.h"

void AItem_Apple::EquipItem()
{
	Super::EquipItem();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("Use Object PickUp: ") + Name);

	Player->GiveFood(15);
	Player->TakeWeight(Weight);
}