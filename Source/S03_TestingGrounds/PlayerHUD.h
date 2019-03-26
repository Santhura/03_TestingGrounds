// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S03_TestingGroundsHUD.h"
#include "PlayerHUD.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS()
class S03_TESTINGGROUNDS_API APlayerHUD : public AS03_TestingGroundsHUD
{
	GENERATED_BODY()
	
	
private:

	UPROPERTY( EditDefaultsOnly, Category = "WeaponUI" )
	UTexture2D* currentWeaponImage;


	
};
