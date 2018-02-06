// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "Item_Canteen.generated.h"

/**
 * 
 */
UCLASS()
class FPSINVENTORY_API AItem_Canteen : public APickupItem
{
	GENERATED_BODY()

public:

	virtual void EquipItem() override;

};