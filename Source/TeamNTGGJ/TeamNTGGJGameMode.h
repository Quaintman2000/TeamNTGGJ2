// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TeamNTGGJGameMode.generated.h"

UCLASS(minimalapi)
class ATeamNTGGJGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATeamNTGGJGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WinCondition)
	int MaxCollectables;

	UPROPERTY(BlueprintReadOnly)
	int CurrentCollectables;

	UFUNCTION(BlueprintCallable)
	void IncrementCurrentCollectables();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWin();
};



