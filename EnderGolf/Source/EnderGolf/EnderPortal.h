// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnderPortal.generated.h"

UCLASS()
class ENDERGOLF_API AEnderPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnderPortal();

	UFUNCTION()
	void TeleportPortal(UPrimitiveComponent* OtherActor,AActor* Actor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void TeleportPortal2(UPrimitiveComponent* OtherActor,AActor* Actor, UPrimitiveComponent* OtherComp, FVector Vector, const FHitResult & SweepResult);
	UPROPERTY(EditAnywhere)
	int32 PortalID;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Elipse;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CollisionBody;
};
