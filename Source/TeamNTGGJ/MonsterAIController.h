// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

class UAIPerceptionComponent;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class TEAMNTGGJ_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Behaviors, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerceptionComp;

public:
	AMonsterAIController();

protected:
	//virtual void OnPossess(APawn* InPawn) override;
	
public:
	UPROPERTY(EditAnywhere, Category = Behaviors)
	UBehaviorTree* MonsterBehaviorTree; 
};
