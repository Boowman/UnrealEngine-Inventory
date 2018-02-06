// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Engine.h"
#include "ProximityManager.h"
#include "InventoryManager.h"
#include "FPSInventoryCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	Mesh->SetSimulatePhysics(true);
	RootComponent = Mesh;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Item Collider"));
	Collider->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Item Collider"));
	Collider->SetSphereRadius(100);
	Collider->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	Collider->bGenerateOverlapEvents = true;

	ItemType = EItemType::TYPE_Item;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AFPSInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	InventoryManager = Player->FindComponentByClass<UInventoryManager>();
	ProximityManager = Player->FindComponentByClass<UProximityManager>();

	InventoryManager->AssignAnItemID(this);
}

void AItem::EquipItem() 
{ 
	if (AttachToSocket)
	{
		Player->AttachItemToSocket(this, EquipType);

		for (int8 i = 0; i < Player->Equipment->EquipmentSlots.Num(); i++)
		{
			if (Mesh->StaticMesh)
			{
				if (Player->Equipment->EquipmentSlots[i]->GetName() == GetEquipTypeName().ToString())
				{
					Player->Equipment->EquipmentSlots[i]->SetStaticMesh(Mesh->StaticMesh);
					break;
				}
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("NULL"));
		}
	}
}

FName AItem::GetEquipTypeName()
{
	switch (EquipType)
	{
	case EEquipType::TYPE_Back:
		return "Back";
		break;
	case EEquipType::TYPE_RightWaist:
		return "RightWaist";
		break;
	case EEquipType::TYPE_LeftWaist:
		return "LeftWaist";
		break;
	case EEquipType::TYPE_RightHand:
		return "RightHand";
		break;
	case EEquipType::TYPE_LeftHand:
		return "LeftHand";
		break;
	default:
		return "None";
		break;
	}

	return "None";
}
