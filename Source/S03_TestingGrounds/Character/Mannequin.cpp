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
	if( IsPlayerControlled() )
	{
		gun->AttachToComponent( FParms, FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "GripPoint" ) );
		gun->AnimInstance1P = FParms->GetAnimInstance();
	}
	else
	{
		gun->AttachToComponent( GetMesh(), FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "GripPoint" ) );
		gun->AnimInstance3P = GetMesh()->GetAnimInstance();
	}

	if( InputComponent != NULL )
	{
		InputComponent->BindAction( "Fire", IE_Pressed, this, &AMannequin::PullTrigger );
	}
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

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	if( gun != NULL )
	{
		gun->AttachToComponent( GetMesh(), FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), TEXT( "GripPoint" ) );
	}
}

void AMannequin::PullTrigger()
{
	gun->OnFire();
}

