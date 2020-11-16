// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnderProjectile_Normal.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &ABaseCharacter::Throw);

}

void ABaseCharacter::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void ABaseCharacter::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void ABaseCharacter::Throw()
{
	FVector MuzzleLocation;
	FRotator MuzzleRotation;
	GetMuzzle(MuzzleLocation, MuzzleRotation);
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn the projectile at the muzzle.
		AEnderProjectile_Normal* Projectile = World->SpawnActor<AEnderProjectile_Normal>(MuzzleLocation, MuzzleRotation, SpawnParams);/*MyEnderProjectileBlueprint*/
		if (Projectile)
		{
			// Set the projectile's initial trajectory.
			FVector LaunchDirection = MuzzleRotation.Vector();
			APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
			Player->SetViewTargetWithBlend(Projectile, 2.f);
			Projectile->FireInDirection(LaunchDirection);
			
		}
	}
}
void ABaseCharacter::GetMuzzle(FVector &MuzzleLocation, FRotator &MuzzleRotation)
{
	UE_LOG(LogTemp, Warning, TEXT("FIRED"));
// Attempt to fire a projectile.
	// Get the camera transform.
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	// Transform MuzzleOffset from camera space to world space.
	MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// Skew the aim to be slightly upwards.
	MuzzleRotation = CameraRotation;
	MuzzleRotation.Pitch += 10.0f;
}