// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor * UActorPool::CheckOut()
{
	if( pool.Num() == 0 )
	{
		return nullptr;
	}
	return pool.Pop();
}

void UActorPool::Return( AActor * actorToReturn )
{
	Add(actorToReturn);
}

void UActorPool::Add( AActor * actorToAdd )
{
	pool.Push( actorToAdd );
}
