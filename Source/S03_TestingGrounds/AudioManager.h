// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"

class UAudioComponent;

UCLASS()
class S03_TESTINGGROUNDS_API AAudioManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = ( AllowPrivateAccess = "true" ) )
	UAudioComponent* audioComponent = nullptr;

	UPROPERTY( EditDefaultsOnly, Category = "Audio")
	float volumeMultiplier = 1;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION( BlueprintCallable, Category = "Audio" )
	float GetVolumeMultiplier()const { return volumeMultiplier; }

	UFUNCTION( BlueprintCallable, Category = "Audio" )
		void SetVolumeMultiplier( float newVolumeMultiplier ) { volumeMultiplier = newVolumeMultiplier; }
	
};
