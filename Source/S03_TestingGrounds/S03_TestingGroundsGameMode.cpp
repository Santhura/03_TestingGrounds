// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "S03_TestingGroundsGameMode.h"
#include "S03_TestingGroundsHUD.h"
#include "Player/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AS03_TestingGroundsGameMode::AS03_TestingGroundsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Character/Behaviour/Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AS03_TestingGroundsHUD::StaticClass();
}
