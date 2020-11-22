// Fill out your copyright notice in the Description page of Project Settings.


#include "EnderProjectile_Normal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnderProjectile_Normal::AEnderProjectile_Normal()
{
	if(CollisionComponent) {

		CollisionComponent->OnComponentHit.AddDynamic(this, &AEnderProjectile_Normal::OnHit);

	}
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if (Player) {
		UCharacterMovementComponent* Movement = Player->GetCharacterMovement();
		if (Movement) 
		{
			// CurrentMode = Movement->GetGroundMovementMode();
			Movement->SetMovementMode(EMovementMode::MOVE_None);
		}
	}
	// CollisionComponent->OnComponentHit.AddDynamic(this, &AEnderProjectile_Normal::OnHit);
}

void AEnderProjectile_Normal::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	CollisionLocation = HitComponent->GetComponentLocation();
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if (Player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Player->GetActorLabel() );

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Player->GetOwner()->GetFName().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Collision: %s"), *CollisionLocation.ToString());

		// CollisionLocation.Z+=50;
		//  Player->SetActorLocation(CollisionLocation, true);
		CollisionLocation.Z +=50;
		//teleports to location, if player does not fit or hits side, teleport to closest available 
		UCharacterMovementComponent* Movement = Player->GetCharacterMovement();
		if (Movement) 
		{
			// UE_LOG(LogTemp, Warning, TEXT("movement test"));
			Movement->SetMovementMode(EMovementMode::MOVE_Walking);
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *(Movement->GetAirControl().ToString()));
			Movement->AirControl = 1.f;
			Movement->GravityScale = 1.f;
		}
		Player->TeleportTo(CollisionLocation, Player->GetActorRotation());
	}
    Destroy();
}