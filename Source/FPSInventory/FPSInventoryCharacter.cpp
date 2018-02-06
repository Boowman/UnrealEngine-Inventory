// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSInventoryCharacter.h"
#include "FPSInventoryProjectile.h"
#include "Item.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/GameEngine.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFPSInventoryCharacter

AFPSInventoryCharacter::AFPSInventoryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/animation blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AFPSInventoryCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	this->GetComponents(Components);

	for (int8 i = 0; i < Components.Num(); i++)
	{
		if (Components[i]->ComponentHasTag("InventoryTablet"))
		{
			BP_Inventory = Cast<UChildActorComponent>(Components[i]);
			break;
		}
	}

	if (BP_Inventory != nullptr)
	{
		BP_Inventory->GetChildrenComponents(true, ChildComponents);

		for (int8 i = 0; i < ChildComponents.Num(); i++)
		{
			if (ChildComponents[i]->ComponentHasTag("UI"))
				WidgetComponent = Cast<UWidgetComponent>(ChildComponents[i]);
		}

		if(WidgetComponent != nullptr)
			UIMenus = Cast<UUI_Menus>(WidgetComponent->GetUserWidgetObject());
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("No Inventory Table Found: FPSInventoryCharacter - 121"));
}

void AFPSInventoryCharacter::ShowCursor()
{
	bShowCursor = !bShowCursor;

	if(PlayerController == nullptr)
		PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController != nullptr)
	{
		PlayerController->SetIgnoreLookInput(bShowCursor);

		PlayerController->bShowMouseCursor = bShowCursor;
		PlayerController->bEnableClickEvents = bShowCursor;
		PlayerController->bEnableMouseOverEvents = bShowCursor;
	}
}

void AFPSInventoryCharacter::AttachItemToSocket(AItem* Item, EEquipType EquipType)
{
	switch (EquipType)
	{
	case EEquipType::TYPE_Back:
		EquipmentMeshList[0]->SetStaticMesh(Item->Mesh->GetStaticMesh());
		break;
	case EEquipType::TYPE_RightWaist:
		EquipmentMeshList[1]->SetStaticMesh(Item->Mesh->GetStaticMesh());
		break;
	case EEquipType::TYPE_LeftWaist:
		EquipmentMeshList[2]->SetStaticMesh(Item->Mesh->GetStaticMesh());
		break;
	case EEquipType::TYPE_RightHand:
		EquipmentMeshList[3]->SetStaticMesh(Item->Mesh->GetStaticMesh());
		break;
	case EEquipType::TYPE_LeftHand:
		EquipmentMeshList[4]->SetStaticMesh(Item->Mesh->GetStaticMesh());
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFPSInventoryCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up game play key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("ShowCursor", IE_Released, this, &AFPSInventoryCharacter::ShowCursor);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFPSInventoryCharacter::OnResetVR);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSInventoryCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSInventoryCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSInventoryCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSInventoryCharacter::LookUpAtRate);
}

void AFPSInventoryCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AFPSInventoryProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AFPSInventoryProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AFPSInventoryCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFPSInventoryCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFPSInventoryCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AFPSInventoryCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AFPSInventoryCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSInventoryCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSInventoryCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSInventoryCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AFPSInventoryCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFPSInventoryCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFPSInventoryCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFPSInventoryCharacter::TouchUpdate);
	}
	return bResult;
}

void AFPSInventoryCharacter::AccessObject(AActor * Actor)
{
	if (Actor != nullptr)
	{
		// Do Stuff with the Actor
	}
}

//////////////////////////////////////////////////////////////////////////
// Player Stats

FText AFPSInventoryCharacter::GetHealth()
{
	return FText::AsNumber(FMath::RoundToInt(health));
}

FText AFPSInventoryCharacter::GetThirst()
{
	return FText::AsNumber(FMath::RoundToInt(thirst));
}

FText AFPSInventoryCharacter::GetFood()
{
	return FText::AsNumber(FMath::RoundToInt(food));
}

FText AFPSInventoryCharacter::GetMoney()
{
	return FText::AsNumber(money);
}

FText AFPSInventoryCharacter::GetWeight()
{
	FString tempStorage = FString::FromInt(currentWeight) + FString("/") + FString::FromInt(maxWeight) + FString(" Kg");

	return FText::FromString(tempStorage);
}

void AFPSInventoryCharacter::GiveHealth(float value)
{
	health += value;

	if (health > 100)
		health = 100;
}

void AFPSInventoryCharacter::GiveWater(float value)
{
	thirst += value;

	if (thirst > 100)
		thirst = 100;
}

void AFPSInventoryCharacter::GiveFood(float value)
{
	food += value;

	if (food > 100)
		food = 100;
}

void AFPSInventoryCharacter::GiveMoney(float value)
{
	money += value;
}

void AFPSInventoryCharacter::TakeHealth(float value)
{
	health -= value;

	if (health < 0)
		health = 0;
}

void AFPSInventoryCharacter::TakeWater(float value)
{
	thirst -= value;

	if (thirst < 0)
		thirst = 0;
}

void AFPSInventoryCharacter::TakeFood(float value)
{
	food -= value;

	if (food < 0)
		food = 0;
}

void AFPSInventoryCharacter::TakeMoney(float value)
{
	money -= value;
}

void AFPSInventoryCharacter::AddWeight(float value)
{
	currentWeight += value;
}

void AFPSInventoryCharacter::TakeWeight(float value)
{
	currentWeight -= value;

	if (currentWeight < 0)
		currentWeight = 0;
}
