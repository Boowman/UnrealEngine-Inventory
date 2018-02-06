// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProximityManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSINVENTORY_API UProximityManager : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proximity, meta = (AllowPrivateAccess = "true"))
	TArray<AItem*> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proximity, meta = (AllowPrivateAccess = "true"))
	TArray<AInventory*> Inventories;

public:
	// Sets default values for this component's properties
	UProximityManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Reference of the Player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proximity)
	class AFPSInventoryCharacter* Player;

	UFUNCTION()
	void AddItem(AItem* Item);

	UFUNCTION()
	void AddInventory(AInventory* Inventory);

	UFUNCTION()
	void RemoveItem(AItem* Item);

	UFUNCTION()
	void RemoveInventory(AInventory* Inventory);

	FORCEINLINE class TArray<AItem*> GetItems() const { return Items; }
};
