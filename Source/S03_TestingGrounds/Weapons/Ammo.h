// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UCLASS()
class S03_TESTINGGROUNDS_API AAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY( EditDefaultsOnly, Category = "Ammo" )
	int32 minAmmoAmount = 10;
	UPROPERTY( EditDefaultsOnly, Category = "Ammo" )
	int32 maxAmmoAmount = 20;
	
	UFUNCTION( BlueprintCallable, Category = "Ammo" )
	int32 GetAmmoAmount() const { return ammoAmount; }

	UFUNCTION( BlueprintCallable, Category = "Ammo" )
	int32 GetMaxAmmoAmount() const { return maxAmmoAmount; }


private:

	int32 ammoAmount = 10;

	float runningTime;
	
	UPROPERTY( EditDefaultsOnly , Category = "Movement")
	float ZValue;

};
