// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Slots.generated.h"

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	SL_BACKPACK UMETA(DisplayName = "Backpack"),
	SL_CHEST	UMETA(DisplayName = "Chest"),
	SL_HEAD		UMETA(DisplayName = "Head"),
	SL_LEGS		UMETA(DisplayName = "Legs"),
	SL_SHOES	UMETA(DisplayName = "Shoes"),
	SL_WEAPON	UMETA(DisplayName = "Weapon")
};

UCLASS()
class FPSINVENTORY_API UUI_Slots : public UUserWidget
{
	GENERATED_BODY()

private:
	float MousePosX;
	float MousePosY;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	uint8 MaximumQuantity;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	class AItem* StoredItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	TArray<AItem*> StoredItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	class AInventory* StoredInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	ESlotType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	uint8 CurrentQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	uint8 SlotID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	bool IsAvailable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	bool IsStorred = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	bool IsDraggable = true;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	UUI_Slots* DescriptionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	UUI_Slots* DragDropSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	class UUI_SlotQuickMenu* QuickMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	class UCanvasPanel* QuantityBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UISlots)
	class UProgressBar* ContentLeftProgress;

public:

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void InitializeSlot(UUserWidget* DescriptionWidget, UUserWidget* DragDropWidget, UUserWidget* QuickMenuWidget);

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void ResetSlot();

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void SelectItem();

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void ShowDragDropSlot(UUI_Slots* Widget, bool bVisibility);

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void ShowDescription(UUI_Slots* Widget, bool bVisibility);

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void ShowQuickMenu(AItem* Item, UUI_Slots* UI_Slot);

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void DropOperation(UUI_Slots* UI_Slot);

	UFUNCTION(BlueprintCallable, Category = UISlots, meta = (AutoCreateRefTerm = "Items"))
	void UpdateSlot(AItem* Item);

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void UpdateQuantityPanel();

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void ContentLeftProgressBar();

private:

	void SwapItem(UUI_Slots* UI_Slot);

	void SwapSlotUpdate(AItem* TempItem, AInventory* TempInventory, TArray<AItem*> TempItems, uint8 TempCurrentQuantity);

	void AddNewItem(UUI_Slots* UI_Slot);

	void MoveSlot(UUI_Slots* UI_Slot);

	void MergeSlots(UUI_Slots* UI_Slot);

	void ReplaceItem(UUI_Slots* UI_Slot);

	void DropCurrentItem();

	UFUNCTION(BlueprintCallable, Category = UISlots)
	void UseCurrentItem();
};