// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveOptions.generated.h"

/**
 * 
 */
UCLASS()
class S03_TESTINGGROUNDS_API USaveOptions : public USaveGame
{
	GENERATED_BODY()


		void Save();
	
};
