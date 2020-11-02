// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnderGolfGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ENDERGOLF_API AEnderGolfGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

	virtual void StartPlay() override;
};
