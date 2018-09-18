// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrollingGuard.h" // TODO remove coupling



EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	Super::ExecuteTask( OwnerComp, NodeMemory );

	// TODO protect against empty patrol routes


	// Get the patrol points
	auto AIController = OwnerComp.GetAIOwner();
	auto controlledPawn = AIController->GetPawn();
	auto patrollingGuard = Cast<APatrollingGuard>( controlledPawn );
	auto patrolPoints = patrollingGuard->patrolPointsCPP;

	// Set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	
	BlackboardComp->SetValueAsObject( waypointKey.SelectedKeyName, patrolPoints[index] );

	auto nextIndex = ( index + 1 ) % patrolPoints.Num();
	BlackboardComp->SetValueAsInt( IndexKey.SelectedKeyName, nextIndex );

	//UE_LOG( LogTemp, Warning, TEXT( "Waypoint index: %i" ), index );
	return EBTNodeResult::Succeeded;
}