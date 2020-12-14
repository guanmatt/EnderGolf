// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindForce.generated.h"

UCLASS()
class ENDERGOLF_API AWindForce : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWindForce();
	bool IsActorOverlapping();
	UPROPERTY(EditAnywhere)
	float Radius = 5.f;
	UPROPERTY(EditAnywhere)
	float Length = 10.f;
	UPROPERTY(EditAnywhere)
	float Force = 1.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
