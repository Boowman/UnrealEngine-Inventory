// Fill out your copyright notice in the Description page of Project Settings.

#include "Item_Axe.h"
#include "Components/SceneComponent.h"
#include "FPSInventoryCharacter.h"
#include "Engine.h"

void AItem_Axe::EquipItem()
{
	Super::EquipItem();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("Use Object PickUp: ") + Name);
}