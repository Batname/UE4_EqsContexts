// Fill out your copyright notice in the Description page of Project Settings.

#include "FindEnemyQueryContext.h"

#include "EqsContextsCharacter.h"
#include "MyAIController.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"


void UFindEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	UE_LOG(LogTemp, Warning, TEXT("ProvideContext"));


	AMyAIController* AICon = Cast<AMyAIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));
	
	if (AICon && AICon->GetSeeingPawn())
	{
		//Set the context SeeingPawn to the provided context data
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, AICon->GetSeeingPawn());
	}

}