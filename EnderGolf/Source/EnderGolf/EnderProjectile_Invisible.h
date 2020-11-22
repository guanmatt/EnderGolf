// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnderProjectile.h"
#include "EnderProjectile_Invisible.generated.h"

/**
 * 
 */
UCLASS()
class ENDERGOLF_API AEnderProjectile_Invisible : public AEnderProjectile
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	AEnderProjectile_Invisible();
	// Called every frame
	void Tick(float DeltaTime) override;
	void DrawTrajectory();

protected:
	void BeginPlay() override;
private:
	FTimerHandle MemberTimerHandle;
};
