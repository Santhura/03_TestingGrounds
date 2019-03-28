// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"



UCLASS()
class S03_TESTINGGROUNDS_API AMannequin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMannequin();

	UPROPERTY( EditDefaultsOnly, Category = "Setup" )
	TSubclassOf<class AGun> gunBlueprint;
	UPROPERTY( EditDefaultsOnly, Category = "Setup" )
		TSubclassOf<class AGun> othergunBlueprint;

	UPROPERTY( EditDefaultsOnly, Category = "Setup" )
	TArray<TSubclassOf<class AGun>> gunBlueprint_list;

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
		AGun* GetCurrentWeapon()const { return currentGun; }

private:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	class UCameraComponent* FP_Camera = nullptr;

	UPROPERTY( VisibleDefaultsOnly, Category = "Mesh" )
	USkeletalMeshComponent* FParms = nullptr;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	AGun* currentGun = nullptr;

	TArray<AGun*> weaponSlots;
	int32 currentWeaponIndex = 0;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	AGun* gun = nullptr;
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	AGun* otherGun = nullptr;

	UPROPERTY( EditAnywhere, Category = "Health" )
	float currentHealth = 100;

	float maxHealth;

	UFUNCTION( BlueprintCallable, Category = "Health" )
	float GetCurrentHealthPercent();

	UFUNCTION( BlueprintCallable, Category = "Health" )
	float GetCurrentHealth() const;

	UFUNCTION( BlueprintCallable, Category = "Health" )
	void SetCurrentHealth(float updatedHealth);

	UFUNCTION( BlueprintCallable, Category = "Weapons" )
	void ChangeWeapons( int32 index, float mouseScrollIndexAddition );


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void UnPossessed() override;


	UFUNCTION( BlueprintCallable, Category = "Input" )
	void PullTrigger();
	
};
