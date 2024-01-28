// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeamNTGGJGameMode.h"
#include "TeamNTGGJCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ATeamNTGGJGameMode::ATeamNTGGJGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void ATeamNTGGJGameMode::SetCurrentCollectables(int NewCurrentCollectables)
{
	CurrentCollectables = NewCurrentCollectables;

	if (CurrentCollectables == MaxCollectables)
		UGameplayStatics::OpenLevel((UObject*)GGameInstance, FName(TEXT("NewMap1")));
}
