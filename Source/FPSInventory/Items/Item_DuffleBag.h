// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Item_DuffleBag.generated.h"

/**
 * 
 */
UCLASS()
class FPSINVENTORY_API AItem_DuffleBag : public AInventory
{
	GENERATED_BODY()
	
protected:
	virtual void EquipItem() override;
};
