// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Inventory.generated.h"

UCLASS()
class FPSINVENTORY_API AInventory : public AItem
{
	GENERATED_BODY()

private:
	//Store the temporary random number
	int32 randomNumber;

public:
	// Sets default values for this actor's properties
	AInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<AItem*> ItemsStored;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	class UUI_Inventory* UI_Inventory;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 MaxSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 UsedSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 MaxWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 CurrentWeight;

public:	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void PickUpItem(AItem* PickedupItem, bool bDragDrop, bool bUpdateUsedSlot = true);

	void DropItem(AItem* StoredItem, bool bUpdateUsedSlot = true);

	bool CheckIfEnoughStorage(TArray<AItem*> Items, AItem* Item);

	void AddWeight(int32 value);

	void TakeWeight(int32 value);
};