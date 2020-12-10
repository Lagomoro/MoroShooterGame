// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoroShooterGameGameMode.h"
#include "MoroShooterGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMoroShooterGameGameMode::AMoroShooterGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
