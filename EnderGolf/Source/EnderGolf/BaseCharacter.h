// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"


#include "BaseCharacter.generated.h"
UCLASS()
class ENDERGOLF_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AEnderProjectile> ProjectileClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Handles input for moving forward and backward.
	UFUNCTION()
	void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
	void MoveRight(float Value);

	// Function that handles firing projectiles.
	UFUNCTION()
	void Throw();
	UFUNCTION()
	void Throw1();
	UFUNCTION()
	void ShowTrajectory();
	UFUNCTION()
	void DrawTrajectory();
	void SetHeld();
	void SetUnheld();
	void GetMuzzle(FVector &MuzzleLocation, FRotator &MuzzleRotation);
	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
	bool bIsHeld = false;
	void DisablePlayerInput();
};
