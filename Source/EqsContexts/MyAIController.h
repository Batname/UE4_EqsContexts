// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class EQSCONTEXTS_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
private:

	/** Blackboard component - used to initialize blackboard values and set/get valuse from blackboard */
	class UBlackboardComponent* BlackboardComp;

	/** BehaviorTreeComponent - used to start behavior tree */
	class UBehaviorTreeComponent* BehaviorTreeComp;

	/** Blackboard key value name */
	const FName BlackboardEnemyKey = FName("Enemy");

	/** The function that fires when the perception of our AI gets updated */
	UFUNCTION()
	void OnPerceptionUpdated(TArray<AActor*> UpdatedActors);

	/** A Sense config for our AI */
	class UAISenseConfig_Sight* Sight;

protected:
	/** The Behavior tree that contains the logic of our AI */
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	/** The Perceptionb Component of our AI */
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* AIPerceptionComponent;

	/** The Perceptionb Component of our AI */
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* AIPerceptionComp;

public:

	AMyAIController();

	virtual void Possess(APawn* InPawn) override;

	/** Return the seeng pawn.Returns null, if our AI has target */
	AActor* GetSeeingPawn();	
	
};
