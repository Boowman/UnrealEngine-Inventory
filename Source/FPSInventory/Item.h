// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	TYPE_Inventory	UMETA(DisplayName = "Inventory"),
	TYPE_Item		UMETA(DisplayName = "Item")
};

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	TYPE_Back			UMETA(DisplayName = "Back"),
	TYPE_RightWaist		UMETA(DisplayName = "RightWaist"),
	TYPE_LeftWaist		UMETA(DisplayName = "LeftWaist"),
	TYPE_RightHand		UMETA(DisplayName = "RightHand"),
	TYPE_LeftHand		UMETA(DisplayName = "LeftHand")
};

UCLASS()
class FPSINVENTORY_API AItem : public AActor
{
	GENERATED_BODY()

private:
	//Store the temporary random number
	int32 randomNumber;

public:
	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when we want to equip the item from the inventory or equip it
	virtual void EquipItem();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	class USphereComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	class UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	bool AttachToSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, meta = (EditCondition = AttachToSocket))
	EEquipType EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	uint8 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	uint8 SlotID;

public:
	// Reference of the Player
	class AFPSInventoryCharacter* Player;

	// Reference of the inventory manager
	class UInventoryManager* InventoryManager;

	// Reference of the proximity manager
	class UProximityManager* ProximityManager;

	FName GetEquipTypeName();
};