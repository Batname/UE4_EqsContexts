// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "EqsContextsCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AMyAIController::AMyAIController()
{
	// Components init
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Create a Sight sence
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	Sight->SightRadius = 1000.f;
	Sight->LoseSightRadius = 1500.f;
	Sight->PeripheralVisionAngleDegrees = 130.f;

	// Tell the SIght sence to detect everything
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	// Register the Sight sence to our Perception component
	AIPerceptionComponent->ConfigureSense(*Sight);	
}


void AMyAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (BehaviorTree != nullptr)
	{
		// Initialize the Blackboard and start the attached behaviour tree
		BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComp->StartTree(*BehaviorTree);
	}
}

void AMyAIController::OnPerceptionUpdated(TArray<AActor*> UpdatedActors)
{
	// If out character exists inside the UpdatedActors array, register him
	// to our blackboard component
	for(AActor* Actor : UpdatedActors)
	{
		if (Actor->IsA<AEqsContextsCharacter>() && !GetSeeingPawn())
		{
			BlackboardComp->SetValueAsObject(BlackboardEnemyKey, Actor);
			return;
		}
	}
}

AActor* AMyAIController::GetSeeingPawn()
{
	// Return the seeing pawn
	UObject* Object = BlackboardComp->GetValueAsObject(BlackboardEnemyKey);

	return Object ? Cast<AActor>(Object) : nullptr;
}