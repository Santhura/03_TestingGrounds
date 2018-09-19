// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"



EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	Super::ExecuteTask( OwnerComp, NodeMemory );
	
	// Get the patrol route	
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto patrolRoute = controlledPawn->FindComponentByClass<UPatrolRoute>();
	if( !ensure( patrolRoute ) ) { return EBTNodeResult::Failed; }

	auto patrolPoints = patrolRoute->GetPatrolPoints();
	if( patrolPoints.Num() == 0 )
	{
		UE_LOG( LogTemp, Warning, TEXT( "There are no patrol points for %s" ), *controlledPawn->GetName());
		return EBTNodeResult::Failed;
	}

	// Set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	
	BlackboardComp->SetValueAsObject( waypointKey.SelectedKeyName, patrolPoints[index] );

	auto nextIndex = ( index + 1 ) % patrolPoints.Num();
	BlackboardComp->SetValueAsInt( IndexKey.SelectedKeyName, nextIndex );

	return EBTNodeResult::Succeeded;
}