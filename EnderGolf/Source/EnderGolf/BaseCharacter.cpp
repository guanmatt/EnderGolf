// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnderProjectile_Normal.h"
#include "EnderProjectile_Sticky.h"
#include "EnderProjectile_Invisible.h"
#include "Engine.h"


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
	DrawTrajectory();
	/*
	FVector MuzzleLocation;
	FRotator MuzzleRotation;
	GetMuzzle(MuzzleLocation, MuzzleRotation);

	TArray<FVector> ProjectilePoints;

	for (float i = 0.f; i<5.0f; i+=0.05)
	{
		ProjectilePoints.Push(MuzzleLocation + MuzzleRotation.Vector().GetSafeNormal() * 
		1000.f * i + 0.5 * FVector(0,0, GetWorld()->GetGravityZ() * 0.5) * i * i);
	}
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;

	for (int32 j = 0; j < ProjectilePoints.Num()-1; j++)
	{
		GetWorld()->LineTraceSingleByChannel(HitResult, ProjectilePoints[j], ProjectilePoints[j+1], ECC_GameTraceChannel1, TraceParams);
		if (HitResult.IsValidBlockingHit()){
			// DrawDebugCircle(GetWorld(), ProjectilePoints[j], 5.0f, 1, FColor::Blue, false, 1, 0, 0);
			// DrawDebugLine(GetWorld(), ProjectilePoints[j], ProjectilePoints[j+1],FColor(255,0,0), false, DeltaTime);
			break;
		}
		DrawDebugLine(GetWorld(), ProjectilePoints[j], ProjectilePoints[j+1], FColor::White, 1, 0, 1);
		// DrawDebugCylinder(GetWorld(), ProjectilePoints[j], ProjectilePoints[j+1], 1.0f, 1, FColor::White, false, DeltaTime*2);
		// DrawDebugPoint(GetWorld(), ProjectilePoints[j], 5.0f, FColor(255,0,0), true);
	}*/
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
	PlayerInputComponent->BindAction("Throw", IE_Released, this, &ABaseCharacter::Throw);
	PlayerInputComponent->BindAction("Throw", IE_Released, this, &ABaseCharacter::SetUnheld);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &ABaseCharacter::SetHeld);
	PlayerInputComponent->BindAction("Throw1", IE_Pressed, this, &ABaseCharacter::Throw1);
	PlayerInputComponent->BindAction("ShowTrajectory", IE_Pressed, this, &ABaseCharacter::SetHeld);
	PlayerInputComponent->BindAction("ShowTrajectory", IE_Released, this, &ABaseCharacter::SetUnheld);


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
void ABaseCharacter::Throw1()
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
		AEnderProjectile_Sticky* Projectile = World->SpawnActor<AEnderProjectile_Sticky>(MuzzleLocation, MuzzleRotation, SpawnParams);/*MyEnderProjectileBlueprint*/
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
	// UE_LOG(LogTemp, Warning, TEXT("FIRED"));
// Attempt to fire a projectile.
	// Get the camera transform.
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
	MuzzleOffset.Set(50.0f, 0.0f, 0.0f);

	// Transform MuzzleOffset from camera space to world space.
	MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// Skew the aim to be slightly upwards.
	MuzzleRotation = CameraRotation;
	MuzzleRotation.Pitch += 10.0f;
}

void ABaseCharacter::ShowTrajectory()
{
	// FVector MuzzleLocation;
	// FRotator MuzzleRotation;
	// GetMuzzle(MuzzleLocation, MuzzleRotation);
	// UWorld* World = GetWorld();
	// if (World)
	// {
	// 	FActorSpawnParameters SpawnParams;
	// 	SpawnParams.Owner = this;
	// 	SpawnParams.Instigator = GetInstigator();

	// 	// Spawn the projectile at the muzzle.
	// 	AEnderProjectile_Invisible* Projectile = World->SpawnActor<AEnderProjectile_Invisible>(MuzzleLocation, MuzzleRotation, SpawnParams);
	// 	if (Projectile)
	// 	{
	// 		// Set the projectile's initial trajectory.
	// 		FVector LaunchDirection = MuzzleRotation.Vector();
	// 		Projectile->FireInDirection(LaunchDirection);
			
	// 	}
	// }
	FVector MuzzleLocation;
	FRotator MuzzleRotation;
	GetMuzzle(MuzzleLocation, MuzzleRotation);
	FPredictProjectilePathParams PredictParams(15.0f, MuzzleLocation, 1000.f * MuzzleRotation.Vector().GetSafeNormal(), 100.0f, ECC_WorldStatic);
	FPredictProjectilePathResult PredictResult;
	bool Prediction = UGameplayStatics::PredictProjectilePath(GetWorld(),PredictParams, PredictResult);
	
	// UE_LOG(LogTemp, Warning, TEXT("muzzle rotation: %s"), *	MuzzleRotation.Vector().GetSafeNormal().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *PredictResult.LastTraceDestination.Location.ToString())
	if (Prediction)
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT SOMETHING"));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("no hit"));
	}
	for (int32 i = 0; i<PredictResult.PathData.Num(); i++)
	{
		DrawDebugPoint(GetWorld(), PredictResult.PathData[i].Location, 5.0f, FColor(255,255,255), false, 0.5f);
	}
	
}


void ABaseCharacter::DrawTrajectory()
{
	// UE_LOG(LogTemp, Warning, TEXT("%d"), bIsHeld);
	if (bIsHeld)
	{
		FVector MuzzleLocation;
		FRotator MuzzleRotation;
		GetMuzzle(MuzzleLocation, MuzzleRotation);

		TArray<FVector> ProjectilePoints;
		// UE_LOG(LogTemp, Warning, TEXT("%f"), GetWorld()->GetDeltaSeconds());
		// UE_LOG(LogTemp, Warning, TEXT("%d"), FApp::GetDeltaTime());
		for (float i = 0.f; i<5.0f; i+=0.05)
		{
			ProjectilePoints.Push(MuzzleLocation + MuzzleRotation.Vector().GetSafeNormal() * 1000.f * i + 0.5 * FVector(0,0, GetWorld()->GetGravityZ() * 0.5) * i * i);
			/*ProjectilePoints.Push(FVector(
				MuzzleLocation.X + MuzzleRotation.Vector().X * i,
				MuzzleLocation.Y + MuzzleRotation.Vector().Y * i, 
				MuzzleLocation.Z + MuzzleRotation.Vector().Z * i + 0.5 * GetWorld()->GetGravityZ() * i * i));
			*/
		}
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		for (int32 j = 0; j < ProjectilePoints.Num()-1; j++)
		{
			GetWorld()->LineTraceSingleByChannel(HitResult, ProjectilePoints[j], ProjectilePoints[j+1], ECC_Visibility, TraceParams);
			// GetWorld()->LineTraceSingleByChannel(HitResult, ProjectilePoints[j], ProjectilePoints[j+1], ECC_GameTraceChannel1, TraceParams);
			// UE_LOG(LogTemp, Warning, TEXT("%d"), HitResult.IsValidBlockingHit());
			if (HitResult.IsValidBlockingHit()){
				// DrawDebugLine(GetWorld(), ProjectilePoints[j], ProjectilePoints[j+1],FColor(255,0,0), true, 2.0f);
				break;
			}
			DrawDebugLine(GetWorld(), ProjectilePoints[j], ProjectilePoints[j+1],FColor(255,255,255), false, GetWorld()->GetDeltaSeconds()+GetWorld()->GetDeltaSeconds()*0.0001);
			// DrawDebugPoint(GetWorld(), ProjectilePoints[j], 5.0f, FColor(255,0,0), true);
		}

	}
}

void ABaseCharacter::SetHeld()
{
	bIsHeld = true;
}
void ABaseCharacter::SetUnheld()
{
	bIsHeld = false;
}