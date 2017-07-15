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
	//Components Init.
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	//Create a Sight Sense
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	Sight->SightRadius = 3000.f;
	Sight->LoseSightRadius = 3500.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;

	//Tell the sight sense to detect everything
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	//Register the sight sense to our Perception Component
	AIPerceptionComp->ConfigureSense(*Sight);
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

	//Register the OnPerceptionUpdated function to fire whenever the AIPerception get's updated
	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPerceptionUpdated);
}

void AMyAIController::OnPerceptionUpdated(TArray<AActor*> UpdatedActors)
{
	//If our character exists inside the UpdatedActors array, register him
	//to our blackboard component
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor->IsA<AEqsContextsCharacter>() && !GetSeeingPawn())
		{
			BlackboardComp->SetValueAsObject(BlackboardEnemyKey, Actor);
			return;
		}
	}

	//The character doesn't exist in our updated actors - so make sure
	//to delete any previous reference of him from the blackboard
	BlackboardComp->SetValueAsObject(BlackboardEnemyKey, nullptr);
}

AActor* AMyAIController::GetSeeingPawn()
{
	// Return the seeing pawn
	UObject* Object = BlackboardComp->GetValueAsObject(BlackboardEnemyKey);

	return Object ? Cast<AActor>(Object) : nullptr;
}