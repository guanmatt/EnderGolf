// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnderProjectile.h"
#include "EnderProjectile_Sticky.generated.h"

/**
 * 
 */
UCLASS()
class ENDERGOLF_API AEnderProjectile_Sticky : public AEnderProjectile
{
	GENERATED_BODY()
public:
	AEnderProjectile_Sticky();
private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
