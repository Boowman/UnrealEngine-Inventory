// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GUI/UI_Slots.h"
#include "Blueprint/UserWidget.h"
#include "UI_SlotQuickMenu.generated.h"

UCLASS()
class FPSINVENTORY_API UUI_SlotQuickMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slot)
	class AItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slot)
	class UUI_Slots* UI_Slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slot)
	class APlayerController* PlayerController;

	class UWidgetLayoutLibrary* Object;
	
public:

	void SetupQuickMenu(AItem* Item, UUI_Slots* UI_Slot);

	UFUNCTION(BlueprintCallable, Category = Slot)
	void DropObject();

	UFUNCTION(BlueprintCallable, Category = Slot)
	void UseObject();
};
