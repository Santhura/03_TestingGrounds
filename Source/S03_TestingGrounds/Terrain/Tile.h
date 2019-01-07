// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()
	FVector location;
	float rotation;
	float scale;
};

USTRUCT( BlueprintType )
struct FActorProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Spawning" )
	int minSpawn = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Spawning" )
	int maxSpawn = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Spawning" )
	float radius = 500;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Spawning" )
	float minScale = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Spawning" )
	float maxScale = 1;
};

class UActorPool;

UCLASS()
class S03_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION( BlueprintCallable, Category = "Spawning" )
	void PlaceActors( TSubclassOf<AActor> toSpawn, FActorProperties actorProperties);

	UFUNCTION( BlueprintCallable, Category = "Spawning" )
	void PlaceAIPawns( TSubclassOf<APawn> toSpawn, FActorProperties actorProperties );
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

	UPROPERTY( EditDefaultsOnly, Category = "Spawning" )
	FVector minExtent;
	UPROPERTY( EditDefaultsOnly, Category = "Spawning" )
	FVector maxExtent;

	UPROPERTY( EditDefaultsOnly, Category = "Navigation" )
	FVector navigationBoundsOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION( BlueprintCallable, Category = "Pool" )
	void SetPool( UActorPool* pool );


private:

	UActorPool* pool;
	AActor* navMeshBoundsVolume;
	bool isTileConquered = false;

	bool CanSpawnAtLocation( FVector location, float radius );
	bool FindEmpyLocation(FVector& outLocation, float radius );

	template<class T>
	void RandomlyPlaceActors( TSubclassOf<T> toSpawn, FActorProperties actorProperties );


	void PlaceActor( TSubclassOf<AActor> toSpawn, FSpawnPosition spawnPosition);
	void PlaceActor( TSubclassOf<APawn> toSpawn, FSpawnPosition spawnPosition );
	void PositionNavMeshBoundsVolume();

	UFUNCTION( BlueprintCallable, Category = "Score" )
	void TileConquered();
};
