// Fill out your copyright notice in the Description page of Project Settings.

#include "Ammo.h"


// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	
	ammoAmount = FMath::RandRange( minAmmoAmount, maxAmmoAmount );
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation();

	float deltaHeight = ( FMath::Sin( runningTime + DeltaTime ) - FMath::Sin( runningTime ) );

	newLocation.Z += deltaHeight * ZValue;
	runningTime += DeltaTime;
	SetActorLocation( newLocation );
}

