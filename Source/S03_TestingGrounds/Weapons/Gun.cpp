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
#include "Character/Mannequin.h"


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
	UE_LOG( LogTemp, Warning, TEXT( "What weapon: %s" ), *GetName() )
		UWorld* const World = GetWorld();
		if( World != NULL )
		{
			if( P_muzzleFlash == nullptr ) return;
			
			UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), P_muzzleFlash, FP_MuzzleLocation->GetComponentLocation(), FRotator(), true );
			
			FHitResult hitResult;

			FVector endLocation = isHoldingByPlayer ? GetEndLineTraceFromFPCamera() : GetEndLineLocation();
			World->LineTraceSingleByChannel( hitResult, FP_MuzzleLocation->GetComponentLocation(), endLocation, ECollisionChannel::ECC_Camera );
			DrawDebugLine(
				GetWorld(),
				FP_MuzzleLocation->GetComponentLocation(),
				endLocation,
				FColor::Red,
				false, 20.f, 0, 1
			);

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

FVector AGun::GetEndLineTraceFromFPCamera() const
{
	FVector playerViewLocation;
	FRotator playerViewRotation;
	if( !playerController ) { return FVector( 0 ); }

	playerController->GetPlayerViewPoint( playerViewLocation, playerViewRotation );
	return playerViewLocation + playerViewRotation.Vector() * ShootingRange;
}

FVector AGun::GetEndLineLocation() const
{
	FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
	FRotator spawnRotation = FP_MuzzleLocation->GetComponentRotation();
	FVector endLoc = SpawnLocation + spawnRotation.Vector() * ShootingRange;

	return endLoc;
}

float AGun::GetRealDamage(FString bodyName) const
{
	float headShotDamage = FMath::FRandRange( 95, 200 );
	float upperBodyDamage = FMath::FRandRange( damageMin, damageMax );
	float lowerBodyDamage = FMath::FRandRange( 5, 10 );

	EBodyParts bodyParts = GetBodyPartHit( bodyPartLists, bodyName );
	switch( bodyParts )
	{
		case EBodyParts::HEAD:
			UE_LOG( LogTemp, Warning, TEXT( "head shot damage: %f" ), headShotDamage );
			return headShotDamage;
			break;
		case EBodyParts::UPPERBODY:
			UE_LOG( LogTemp, Warning, TEXT( "upperbody shot damage: %f" ), upperBodyDamage );
			return upperBodyDamage;
			break;
		case EBodyParts::LOWERBODY:
			UE_LOG( LogTemp, Warning, TEXT( "lowerbody shot damage: %f" ), lowerBodyDamage );
			return lowerBodyDamage;
			break;
		case EBodyParts::NONE:
			UE_LOG( LogTemp, Warning, TEXT( "NONE shot damage" ) );
			return FMath::FRandRange( 10, 50 );
			break;
		default:
			break;
	}

	return 0;
}

EBodyParts AGun::GetBodyPartHit(FBodyPartLists searchBodyPart, FString bodyName ) const
{
	for( size_t i = 0; i < searchBodyPart.headBodyParts.Num(); i++ )
	{
		if( bodyName.Contains( searchBodyPart.headBodyParts[i] ) )
		{
			return EBodyParts::HEAD;
		}
	}
	for( size_t i = 0; i < searchBodyPart.upperBodyParts.Num(); i++ )
	{
		if( bodyName.Contains( searchBodyPart.upperBodyParts[i] ) )
		{
			return EBodyParts::UPPERBODY;
		}
	}
	for( size_t i = 0; i < searchBodyPart.lowerBodyParts.Num(); i++ )
	{
		if( bodyName.Contains( searchBodyPart.lowerBodyParts[i] ) )
		{
			return EBodyParts::LOWERBODY;
		}
	}
	return EBodyParts::NONE;
}