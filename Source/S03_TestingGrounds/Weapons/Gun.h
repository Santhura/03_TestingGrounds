// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UENUM()
enum class EBodyParts : uint8
{
	NONE,
	HEAD,
	UPPERBODY,
	LOWERBODY
};

UENUM()
enum class EGunType : uint8
{
	NONE,
	RIFLE,
	GRENATE_LAUNCHER

};

USTRUCT( BlueprintType )
struct FBodyPartLists
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body" )
		TArray<FString> upperBodyParts;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body" )
		TArray<FString> lowerBodyParts;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body" )
		TArray<FString> headBodyParts;
};

class ADecalActor;
class UParticleSystem;
class USkeletalMeshComponent;
class UTexture2D;

UCLASS()
class S03_TESTINGGROUNDS_API AGun : public AActor
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY( VisibleDefaultsOnly, Category = Mesh )
	USkeletalMeshComponent* Mesh1P;
	
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY( VisibleDefaultsOnly, Category = Mesh )
	USkeletalMeshComponent* FP_Gun;

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

	UPROPERTY( EditDefaultsOnly, Category = "Texture" )
	UTexture2D* WeaponTex;

	UFUNCTION( BlueprintCallable, Category = "Texture" )
	UTexture2D* GetWeaponTex()const { return WeaponTex; };

	UFUNCTION( BlueprintCallable, Category = "Ammo" )
		int32 GetAmmo()const { return ammo; }
	UFUNCTION( BlueprintCallable, Category = "Ammo" )
		void RefillAmmo(int32 addAmmo);

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

	EGunType GetGunType()const { return gunType; };
	float GetFireRate() const { return fireRate; }

private:
	UPROPERTY( EditDefaultsOnly, Category = "Ammo" )
	int32 ammo = 20;
	UPROPERTY( EditDefaultsOnly, Category = "Ammo" )
	int32 maxAmmo = 20;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float damageMin = 10;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float damageMax = 40;

	float damage = 30;

	UPROPERTY( EditDefaultsOnly, Category = "Gun" )
	float ShootingRange = 100000;
	UPROPERTY( EditDefaultsOnly, Category = "Gun" )
		float fireRate = .1f;

	UFUNCTION( BlueprintCallable, Category = "Body" )
		void SetBodyPartList( FBodyPartLists newbodyPartLists ) { bodyPartLists = newbodyPartLists; }

	UPROPERTY( EditDefaultsOnly, Category = "Gun")
	TEnumAsByte<EGunType> gunType;

	FBodyPartLists bodyPartLists;

	FVector GetEndLineTraceFromFPCamera() const;
	FVector GetEndLineLocation() const;
	EBodyParts GetBodyPartHit(FBodyPartLists searchBodyPart, FString bodyName ) const;

	float GetRealDamage( FString bodyName ) const;

	class APlayerController* playerController;
};
