// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseNextWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class S03_TESTINGGROUNDS_API UChooseNextWaypoint : public UBTTaskNode
{
	GENERATED_BODY()
	
	
protected:

	UPROPERTY( EditAnywhere, Category = "Blackboard" )
	struct FBlackboardKeySelector IndexKey;

	UPROPERTY( EditAnywhere, Category = "Blackboard" )
	struct FBlackboardKeySelector waypointKey;

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;
	
};
