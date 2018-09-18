// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"




EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	Super::ExecuteTask( OwnerComp, NodeMemory );
	UE_LOG( LogTemp, Warning, TEXT( "Donkey: C++ choose way point succeeded" ) );
	return EBTNodeResult::Succeeded;
}