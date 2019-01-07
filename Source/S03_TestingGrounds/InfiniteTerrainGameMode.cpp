// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "S03_TestingGroundsHUD.h"
#include "Player/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "ActorPool.h"


AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
	:Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder( TEXT( "/Game/Dynamic/Character/Behaviour/BP_Character" ) );
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AS03_TestingGroundsHUD::StaticClass();

	navMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>( FName( "Nav mesh bounds volume pool" ) );
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto volumeIterator = TActorIterator<ANavMeshBoundsVolume>( GetWorld() );
	while( volumeIterator )
	{
		AddToPool( *volumeIterator );
		++volumeIterator;
	}
}

void AInfiniteTerrainGameMode::NewNewTileConquered()
{
	score++;
}

void AInfiniteTerrainGameMode::AddToPool( ANavMeshBoundsVolume * volumeToAdd )
{
	navMeshBoundsVolumePool->Add( volumeToAdd );
}

int32 AInfiniteTerrainGameMode::GetScore() const
{
	return score;
}

