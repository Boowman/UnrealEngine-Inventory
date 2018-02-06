// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Components/ActorComponent.h"
#include "InventoryManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSINVENTORY_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<AInventory*> InventoryList;

	// Reference of the Player
	class AFPSInventoryCharacter* Player;
public:
	// Sets default values for this component's properties
	UInventoryManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	TArray<AInventory*> GetInventoryList();

	void AddInventory(AInventory* Inventory);

	void DropInventory(AInventory* Inventory);

	void AssignAnItemID(AItem* Item);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<int32> ItemsID;
};
