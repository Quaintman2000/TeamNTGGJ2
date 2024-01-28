// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"

AMonsterAIController::AMonsterAIController()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
}

//void AMonsterAIController::OnPossess(APawn* InPawn)
//{
//	/*RunBehaviorTree(MonsterBehaviorTree);
//
//	FString message = this->GetActorNameOrLabel() + " Possed: " + InPawn->GetActorNameOrLabel();
//
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, (message));
//	}*/
//}
