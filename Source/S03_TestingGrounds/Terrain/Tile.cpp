// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> toSpawn, int minSpawn, int maxSpawn, float radius, float minScale, float maxScale )
{
	int numberToSpawn = FMath::RandRange( minSpawn, maxSpawn );

	for( size_t i = 0; i < numberToSpawn; i++ )
	{
		FVector spawnPoint;
		float randomScale = FMath::RandRange( minScale, maxScale);
		bool found = FindEmpyLocation( spawnPoint, radius * randomScale );
		if(found)
		{
			float randomRotation = FMath::RandRange( -180.f, 180.f );
			PlaceActor( toSpawn, spawnPoint, randomRotation, randomScale );
		}
	}
}

bool ATile::FindEmpyLocation( FVector& outLocation, float radius )
{
	FVector minPoint = FVector(0, -2000, 0);
	FVector maxPoint = FVector(4000, 2000, 0);
	FBox bounds ( minPoint, maxPoint );
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

void ATile::PlaceActor( TSubclassOf<AActor> toSpawn, FVector spawnPoint, float rotation, float scale )
{
	AActor* spawned = GetWorld()->SpawnActor<AActor>( toSpawn );
	spawned->SetActorRelativeLocation( spawnPoint );
	spawned->AttachToActor( this, FAttachmentTransformRules( EAttachmentRule::KeepRelative, false ) );
	spawned->SetActorRotation( FRotator( 0, rotation, 0 ) );
	spawned->SetActorScale3D(FVector(scale) );
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
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



