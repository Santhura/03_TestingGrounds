// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "BallProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Math/RotationMatrix.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "Classes/Materials/MaterialInterface.h"
#include "Classes/Particles/ParticleSystem.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "FP_Gun" ) );
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment( RootComponent );

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>( TEXT( "MuzzleLocation" ) );
	FP_MuzzleLocation->SetupAttachment( FP_Gun );
	FP_MuzzleLocation->SetRelativeLocation( FVector( 0.2f, 48.4f, -10.6f ) );

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector( 100.0f, 0.0f, 10.0f );
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	playerController = GetWorld()->GetFirstPlayerController();

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::OnFire()
{
	// try and fire a projectile
	if( ProjectileClass != NULL )
	{

		UWorld* const World = GetWorld();
		if( World != NULL )
		{
			if( P_muzzleFlash == nullptr ) return;
			
			UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), P_muzzleFlash, FP_MuzzleLocation->GetComponentLocation(), FRotator(), true );
			
			FHitResult hitResult;

			FVector endLocation = isHoldingByPlayer ? GetEndLineTrace() : GetEndLineLocation();
			World->LineTraceSingleByChannel( hitResult, FP_MuzzleLocation->GetComponentLocation(), endLocation, ECollisionChannel::ECC_Camera );

			//DrawDebugLine(
			//	GetWorld(),
			//	FP_MuzzleLocation->GetComponentLocation(),
			//	endLocation,
			//	FColor::Red,
			//	false, .2f, 0, 1
			//);
			
			if( hitResult.GetActor() )
			{
				if( hitResult.GetActor()->GetName().Contains( "BP_Character" ) || hitResult.GetActor()->GetName().Contains( "player" ) )
				{
					damage = GetRealDamage(hitResult.BoneName.ToString());
					UGameplayStatics::ApplyPointDamage( hitResult.GetActor(), damage, hitResult.Normal, hitResult, nullptr, nullptr, NULL );
				}
				else
				{

					UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), P_bulletImpact, hitResult.Location, FRotator(), true );
					ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>( hitResult.Location, FRotator() );

					if( P_bulletImpact == nullptr ) { return; }

					if( decal )
					{
						decal->SetDecalMaterial( decalMaterial );
						decal->SetLifeSpan( 5.0f );
						decal->GetDecal()->DecalSize = FVector( 12, 12, 12 );
					}

				}
			}

			//const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
			//	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			//const FVector SpawnLocation =  FP_MuzzleLocation->GetComponentLocation();

			//	//Set Spawn Collision Handling Override
			//	FActorSpawnParameters ActorSpawnParams;
			//	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			//	// spawn the projectile at the muzzle
			//	World->SpawnActor<ABallProjectile>( ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams );
		}
	}

	// try and play the sound if specified
	if( FireSound != NULL )
	{
		UGameplayStatics::PlaySoundAtLocation( this, FireSound, GetActorLocation() );
	}

	// try and play a firing animation if specified
	if( FireAnimation3P != nullptr && AnimInstance3P != nullptr )
	{
		AnimInstance3P->Montage_Play( FireAnimation3P, 1.f );
	}

	if( FireAnimation1P != nullptr && AnimInstance1P != nullptr )
	{
		AnimInstance1P->Montage_Play( FireAnimation1P, 1.f );
	}
}

FVector AGun::GetEndLineTrace() const
{
	FVector playerViewLocation;
	FRotator playerViewRotation;
	if( !playerController ) { return FVector( 0 ); }

	playerController->GetPlayerViewPoint( playerViewLocation, playerViewRotation );
	return playerViewLocation + playerViewRotation.Vector() * 100000;
}

FVector AGun::GetEndLineLocation() const
{
	FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
	FRotator spawnRotation = FP_MuzzleLocation->GetComponentRotation();
	FVector endLoc = SpawnLocation + spawnRotation.Vector() * 100000;

	return endLoc;
}

float AGun::GetRealDamage(FString bodyName) const
{
	float headShotDamage = FMath::FRandRange( 95, 200 );
	float upperBodyDamage = FMath::FRandRange( damageMin, damageMax );
	float lowerBodyDamage = FMath::FRandRange( 5, 10 );

	for( size_t i = 0; i < headBodyParts.Num(); i++ )
	{
		if( bodyName.Contains( headBodyParts[i] ) )
		{
			return headShotDamage;
		}
	}

	for( size_t i = 0; i < upperBodyParts.Num(); i++ )
	{
		if( bodyName.Contains( upperBodyParts[i] ) )
		{
			return upperBodyDamage;
		}
	}

	for( size_t i = 0; i < lowerBodyParts.Num(); i++ )
	{
		if( bodyName.Contains( lowerBodyParts[i] ) )
		{
			return lowerBodyDamage;
		}
	}

	return upperBodyDamage;
}
