// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSInventoryGameMode.h"
#include "FPSInventoryHUD.h"
#include "FPSInventoryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSInventoryGameMode::AFPSInventoryGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSInventoryHUD::StaticClass();
}
