// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSInventoryHUD.generated.h"

UCLASS()
class AFPSInventoryHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFPSInventoryHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:

};

