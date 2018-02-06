// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUI/UI_Menus.h"
#include "EquipmentManager.h"
#include "InventoryManager.h"
#include "GameFramework/Character.h"
#include "FPSInventoryCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class AFPSInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	AFPSInventoryCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSInventoryProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cursor)
	bool bShowCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cursor)
	class APlayerController* PlayerController;

public:

	//Assign in blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
	UInventoryManager* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
	UEquipmentManager* Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	UUI_Menus* UIMenus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TArray<UActorComponent*> Components;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	UChildActorComponent* BP_Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TArray<USceneComponent*> ChildComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> EquipmentMeshList;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = true))
	uint8 maxWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = true))
	uint8 currentWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = true))
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = true))
	float thirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = true))
	float food;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = true))
	float money;

public:
	UFUNCTION(BlueprintCallable, Category = Stats)
	FText GetHealth();

	UFUNCTION(BlueprintCallable, Category = Stats)
	FText GetThirst();

	UFUNCTION(BlueprintCallable, Category = Stats)
	FText GetFood();

	UFUNCTION(BlueprintCallable, Category = Stats)
	FText GetMoney();

	UFUNCTION(BlueprintCallable, Category = Stats)
	FText GetWeight();

	UFUNCTION(BlueprintCallable, Category = Stats)
	void GiveHealth(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void GiveWater(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void GiveFood(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void GiveMoney(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void TakeHealth(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void TakeWater(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void TakeFood(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void TakeMoney(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void AddWeight(float value);

	UFUNCTION(BlueprintCallable, Category = Stats)
	void TakeWeight(float value);

	UFUNCTION()
	void ShowCursor();

	UFUNCTION()
	void AttachItemToSocket(AItem* Item, EEquipType EquipType);

protected:

	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};

	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	* Configures input for touchscreen devices if there is a valid touch interface for doing so
	*
	* @param	InputComponent	The input component pointer to bind controls to
	* @returns true if touch controls were enabled.
	*/
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	UFUNCTION(BlueprintCallable, Category = Mesh)
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

public:
	UFUNCTION(BlueprintCallable, Category = Default)
	void AccessObject(AActor* Actor);


};

