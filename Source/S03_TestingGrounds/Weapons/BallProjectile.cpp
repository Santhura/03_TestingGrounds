// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BallProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ABallProjectile::ABallProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	impactExplosion = CreateDefaultSubobject<UParticleSystemComponent>( FName( "Impact Explosion" ) );
	impactExplosion->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );
	impactExplosion->bAutoActivate = false;

	explosionForce = CreateDefaultSubobject<URadialForceComponent>( FName( "Explosion force Component" ) );
	explosionForce->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ABallProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentHit.AddDynamic( this, &ABallProjectile::OnHit );
}

void ABallProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ABallProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	impactExplosion->Activate();
	explosionForce->FireImpulse();
	SetRootComponent( impactExplosion );
	CollisionComp->DestroyComponent();

	UE_LOG( LogTemp, Warning, TEXT( "radius: %f" ), explosionForce->Radius );

	UGameplayStatics::ApplyRadialDamage( 
		this, 
		projectileDamage, 
		GetActorLocation(), 
		explosionForce->Radius, 
		UDamageType::StaticClass(), 
		TArray<AActor*>() 
	);

	DrawDebugSphere( GetWorld(), GetActorLocation(), explosionForce->Radius, 32, FColor::Green, false, 10, 0, 1 );

	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer( timer, this, &ABallProjectile::OnTimerExpire, destoryDelay, false );

}

void ABallProjectile::OnTimerExpire()
{
	Destroy();
}
