// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Menus.generated.h"

UCLASS()
class FPSINVENTORY_API UUI_Menus : public UUserWidget
{
	GENERATED_BODY()

private:
	float CurrentTime;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Menus")
	TArray<UUserWidget*> ProximityList;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Menus")
	TArray<UUserWidget*> InventoryList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Menus")
	class AFPSInventoryCharacter* Character;

public:
	UFUNCTION(BlueprintCallable, Category = "UI Menus")
	float GetCurrentTime();

	UFUNCTION(BlueprintCallable, Category = "UI Menus")
	void InitializeUIMenus(UWrapBox* ProximityListParent, UWrapBox* InventoryListParent, UUserWidget* DescriptionWidget, UUserWidget* DragDropWidget, UUserWidget* QuickMenuWidget);
};
