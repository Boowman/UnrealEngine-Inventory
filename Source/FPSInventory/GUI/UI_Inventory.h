// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "UI_Inventory.generated.h"

//**Forward Declaration because USizeBox* Wasn't Being Picked Up**//
class USizeBox;

UCLASS()
class FPSINVENTORY_API UUI_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	AInventory* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 MaxSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<UUserWidget*> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	USizeBox* SizeBox;

public:

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void InitializeInventory(UWrapBox* SlotsListParent, USizeBox* SizeBoxWidget);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void ShowAvailableSlots();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AssignInventoryToSlots();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void DropInventory();

public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	FText DisplayWeight();
};

