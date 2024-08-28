// Copyright Epic Games, Inc. All Rights Reserved.

#include "PJ_Game/GameGameMode.h"
#include "Player/GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameGameMode::AGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
