// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Camera/CameraComponent.h"
#include "../Weapons/Gun.h"

// Sets default values
AMannequin::AMannequin()
{
	//GetCapsuleComponent()->InitCapsuleSize( 55.f, 96.0f );

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FP_Camera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FP_Camera" ) );
	FP_Camera->SetupAttachment( RootComponent );
	FP_Camera->RelativeLocation = FVector( -39.56f, 1.75f, 64.f );
	FP_Camera->bUsePawnControlRotation = true;

	FParms = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "FP_Arms" ) );
	FParms->SetOnlyOwnerSee( true );
	FParms->SetupAttachment( FP_Camera );
	FParms->bCastDynamicShadow = false;
	FParms->CastShadow = false;
	FParms->RelativeRotation = FRotator( 1.9f, -19.19f, 5.2f );
	FParms->RelativeLocation = FVector( -0.5f, -4.4f, -155.7f );


}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if( gunBlueprint == NULL )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Gun blueprint missing." ) );
		return;
	}
	gun = GetWorld()->SpawnActor<AGun>( gunBlueprint );
	otherGun = GetWorld()->SpawnActor<AGun>( othergunBlueprint );
	weaponSlots.Add( gun );
	weaponSlots.Add( otherGun );
	currentGun = weaponSlots[currentWeaponIndex];

	if( IsPlayerControlled() )
	{
		currentGun->AttachToComponent( FParms, FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "GripPoint" ) );
		currentGun->AnimInstance1P = FParms->GetAnimInstance();
		currentGun->isHoldingByPlayer = true;

		otherGun->AttachToComponent( FParms, FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "OtherGun" ) );
		otherGun->AnimInstance1P = FParms->GetAnimInstance();
		otherGun->isHoldingByPlayer = true;
	}
	else
	{
		gun->AttachToComponent( GetMesh(), FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "GripPoint" ) );
		gun->AnimInstance3P = GetMesh()->GetAnimInstance();
		gun->isHoldingByPlayer = false;
	}

	if( InputComponent != NULL )
	{
		InputComponent->BindAction( "Fire", IE_Pressed, this, &AMannequin::PullTrigger );
		InputComponent->BindAction( "Fire", IE_Released, this, &AMannequin::TriggerRelease );
	}

	maxHealth = currentHealth;
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*PlayerInputComponent->BindAction( "Weapon1", IE_Pressed, this, &AMannequin::SwitchWeapon, 0 );
	PlayerInputComponent->BindAction( "Weapon2", IE_Pressed, this, &AMannequin::SwitchWeapon, 1 );*/

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	if( gun != NULL )
	{
		gun->AttachToComponent( GetMesh(), FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "GripPoint" ) );
	}
}



float AMannequin::GetCurrentHealthPercent()
{
	return currentHealth / maxHealth;
}

float AMannequin::GetCurrentHealth() const
{
	return currentHealth;
}

void AMannequin::SetCurrentHealth( float updatedHealth )
{
	currentHealth = updatedHealth;
}

void AMannequin::ChangeWeapons( int32 index, float mouseScrollIndexAddition )
{
	int32 newIndex = ( mouseScrollIndexAddition < -1 ) ? index : currentWeaponIndex += mouseScrollIndexAddition;
	
	if( newIndex >= weaponSlots.Num() )
		newIndex = 0;
	else if( newIndex < 0 )
		newIndex = weaponSlots.Num() - 1;

	currentWeaponIndex = newIndex;

	if( weaponSlots.IsValidIndex( newIndex ) )
	{
		if( IsPlayerControlled() )
		{
			// detach current gun and attach it to socket on the back
			currentGun->DetachFromActor( FDetachmentTransformRules::KeepWorldTransform );
			currentGun->AttachToComponent( FParms, FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "OtherGun" ) );

			// attach weapon from the slots to the current weapon holding
			currentGun = nullptr;
			currentGun = weaponSlots[newIndex];
			currentGun->AttachToComponent( FParms, FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "GripPoint" ) );
			currentGun->AnimInstance1P = FParms->GetAnimInstance();
			currentGun->isHoldingByPlayer = true;
		}
	}
}

void AMannequin::Shoot()
{
	currentGun->OnFire();
}

void AMannequin::PullTrigger()
{
	if( currentGun->GetGunType() == EGunType::RIFLE )
	{
		firstShot = true;
		if( firstShot )
			Shoot();

		GetWorldTimerManager().SetTimer( handle, this, &AMannequin::Shoot, currentGun->GetFireRate(), true );
	}
	else
	{
		Shoot();
	}
}

void AMannequin::TriggerRelease()
{
	firstShot = false;
	GetWorldTimerManager().ClearTimer( handle );
}
