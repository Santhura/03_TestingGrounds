// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class ADecalActor;
class UParticleSystem;

enum BodyParts
{
	Head,
	UpperBody,
	LowerBody
};

UCLASS()
class S03_TESTINGGROUNDS_API AGun : public AActor
{
	GENERATED_BODY()

		/** Pawn mesh: 1st person view (arms; seen only by self) */
		UPROPERTY( VisibleDefaultsOnly, Category = Mesh )
		class USkeletalMeshComponent* Mesh1P;
	
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY( VisibleDefaultsOnly, Category = Mesh )
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY( VisibleDefaultsOnly, Category = Mesh )
	class USceneComponent* FP_MuzzleLocation;


public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Fires a projectile. */

public:	

	bool isHoldingByPlayer = false;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	FVector GunOffset;

	UPROPERTY( EditDefaultsOnly, Category = "Projectile" )
	class UMaterialInterface* decalMaterial;

	UPROPERTY( EditDefaultsOnly, Category = "Projectile" )
	UParticleSystem* P_bulletImpact;

	UPROPERTY( EditDefaultsOnly, Category = "Projectile" )
	UParticleSystem* P_muzzleFlash;

	/** Projectile class to spawn */
	UPROPERTY( EditDefaultsOnly, Category = Projectile )
	TSubclassOf<class ABallProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	class UAnimMontage* FireAnimation3P;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	class UAnimMontage* FireAnimation1P;
	
	UPROPERTY()
	class UAnimInstance* AnimInstance3P;

	UPROPERTY()
	class UAnimInstance* AnimInstance1P;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnFire();

private:
	
	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float damageMin = 10;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float damageMax = 40;

	float damage = 30;

	UPROPERTY( EditDefaultsOnly, Category = "Body" )
	TArray<FString> upperBodyParts;
	UPROPERTY( EditDefaultsOnly, Category = "Body" )
	TArray<FString> lowerBodyParts;
	UPROPERTY( EditDefaultsOnly, Category = "Body" )
	TArray<FString> headBodyParts;

	FVector GetEndLineTrace() const;
	FVector GetEndLineLocation() const;

	float GetRealDamage( FString bodyName ) const;

	class APlayerController* playerController;
};
