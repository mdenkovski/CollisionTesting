// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollisionTestingGameMode.h"
#include "CollisionTestingCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACollisionTestingGameMode::ACollisionTestingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
