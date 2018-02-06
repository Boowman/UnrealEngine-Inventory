// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "PickupItem.generated.h"

UCLASS()
class FPSINVENTORY_API APickupItem : public AItem
{
	GENERATED_BODY()

private:
	// Reference of the Player
	class AInventory* Inventory;

public:
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	uint8 MaxQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	bool Reusable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp, meta = (EditCondition = "Reusable"))
	float ContentLeft;

public: 
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};