// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallProjectile.generated.h"

class UParticleSystemComponent;
class URadialForceComponent;

UCLASS(config=Game)
class ABallProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;



public:
	ABallProjectile();

	virtual void Tick( float DeltaTime ) override;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY( EditDefaultsOnly, Category = "Projectile" )
		UParticleSystemComponent* impactExplosion = nullptr;

	UPROPERTY( EditDefaultsOnly, Category = "Components" )
		URadialForceComponent* explosionForce = nullptr;

	UFUNCTION( BlueprintCallable, Category = "explosion" )
		void OnTimerExpire();

	float projectileDamage = 80;
	float destoryDelay = .5f;
};

