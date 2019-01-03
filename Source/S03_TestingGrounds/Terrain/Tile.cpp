// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"



// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	minExtent = FVector( 0, -2000, 0 );
	maxExtent = FVector( 4000, 2000, 0 );
	navigationBoundsOffset = FVector( 2000, 0, 0 );
}

void ATile::SetPool( UActorPool * inPool )
{
	UE_LOG( LogTemp, Warning, TEXT( "Setting pool %s" ), *( inPool->GetName() ) )
	pool = inPool;

	PositionNavMeshBoundsVolume();

}

void ATile::PositionNavMeshBoundsVolume()
{
	navMeshBoundsVolume = pool->CheckOut();
	if( navMeshBoundsVolume == nullptr )
	{
		UE_LOG( LogTemp, Error, TEXT( "[%s] Not enough actors in pool" ), *GetName() );
			return;
	}

	UE_LOG( LogTemp, Warning, TEXT( "[%s] checked out: {%s}" ), *GetName(), *navMeshBoundsVolume->GetName() );

	navMeshBoundsVolume->SetActorLocation( GetActorLocation() + navigationBoundsOffset );
	GetWorld()->GetNavigationSystem()->Build();
}



void ATile::PlaceActors(TSubclassOf<AActor> toSpawn, FActorProperties actorProperties )
{
	TArray<FSpawnPosition> spawnPositions = RandomSpawnPositions( actorProperties );

	for( FSpawnPosition spawnPosition : spawnPositions )
	{
		PlaceActor( toSpawn, spawnPosition );
	}
}

TArray<FSpawnPosition> ATile::RandomSpawnPositions( FActorProperties actorProperties )
{
	TArray<FSpawnPosition> spawnPositions;

	int numberToSpawn = FMath::RandRange( actorProperties.minSpawn, actorProperties.maxSpawn );

	for( size_t i = 0; i < numberToSpawn; i++ )
	{
		FSpawnPosition spawnPosition;
		spawnPosition.scale = FMath::RandRange( actorProperties.minScale, actorProperties.maxScale );
		bool found = FindEmpyLocation( spawnPosition.location, actorProperties.radius * spawnPosition.scale );
		if( found )
		{
			spawnPosition.rotation = FMath::RandRange( -180.f, 180.f );
			spawnPositions.Add( spawnPosition );
		}
	}

	return spawnPositions;
}



bool ATile::FindEmpyLocation( FVector& outLocation, float radius )
{
	FBox bounds ( minExtent, maxExtent);
	const int MAX_ATTEMPS = 100;
	for( size_t i = 0; i < MAX_ATTEMPS; i++ )
	{
		FVector candidatePoint = FMath::RandPointInBox( bounds );
		if( CanSpawnAtLocation( candidatePoint, radius ) )
		{
			outLocation = candidatePoint;
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor( TSubclassOf<AActor> toSpawn, FSpawnPosition spawnPosition )
{
	AActor* spawned = GetWorld()->SpawnActor<AActor>( toSpawn );
	spawned->SetActorRelativeLocation( spawnPosition.location );
	spawned->AttachToActor( this, FAttachmentTransformRules( EAttachmentRule::KeepRelative, false ) );
	spawned->SetActorRotation( FRotator( 0, spawnPosition.rotation, 0 ) );
	spawned->SetActorScale3D(FVector(spawnPosition.scale) );
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay( const EEndPlayReason::Type endPlayReason )
{
	Super::EndPlay(endPlayReason);
	pool->Return( navMeshBoundsVolume );

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CanSpawnAtLocation( FVector location, float radius )
{
	FHitResult hitResult;
	FVector globalLocation = ActorToWorld().TransformPosition( location );
	bool hasHit = GetWorld()->SweepSingleByChannel(
		hitResult,
		globalLocation,
		globalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere( radius )
	);

	/*FColor color = hasHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule( GetWorld(), globalLocation, 0, radius, FQuat::Identity, color, true, 100 );*/
	return !hasHit;
}


