// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteTerrainGameMode.generated.h"


/**
 * 
 */
UCLASS()
class S03_TESTINGGROUNDS_API AInfiniteTerrainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	AInfiniteTerrainGameMode();
	
	UFUNCTION( BlueprintCallable, Category = "Bounds Pool" )
	void PopulateBoundsVolumePool();

	int32 score = 0;

	UFUNCTION()
	void NewNewTileConquered();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool" )
	class UActorPool* navMeshBoundsVolumePool;

private:
	void AddToPool(class ANavMeshBoundsVolume *volumeToAdd);
};
