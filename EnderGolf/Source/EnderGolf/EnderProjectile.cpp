// Fill out your copyright notice in the Description page of Project Settings.
#include "EnderProjectile.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Blueprint.h"
#include "GameFramework/Character.h"


// Sets default values
AEnderProjectile::AEnderProjectile()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
		// RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	}
	
   	if(!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		CollisionComponent->InitSphereRadius(1.0f);//15.0f
		CollisionComponent->SetNotifyRigidBodyCollision(true);
		CollisionComponent->SetSimulatePhysics(true);

		// UE_LOG(LogTemp, Warning, TEXT("physics: %d"), CollisionComponent->IsSimulatingPhysics());
		// Set the root component to be the collision component.
		RootComponent=CollisionComponent;
		// CollisionComponent->SetupAttachment(RootComponent);
	}
	// // Set the sphere's collision profile name to "Projectile".
	// ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	if(!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
	}
	if(!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/SphereUpdated.SphereUpdated'"));
		if(Mesh.Succeeded())
		{
			// UE_LOG(LogTemp, Warning, TEXT("MESH SUCCESS"));
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}



	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/SphereMaterial.SphereMaterial'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	// ProjectileMeshComponent->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);
	// ProjectileMeshComponent->SetNotifyRigidBodyCollision(true);
	// ProjectileMeshComponent->SetSimulatePhysics(true);
	// ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->AttachTo(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	// Camera->SetupAttachment(RootComponent);
	
	// UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
	// Primitive->SetNotifyRigidBodyCollision(true);
	// Primitive->SetSimulatePhysics(true);
	// UE_LOG(LogTemp, Warning, TEXT("notify collision: %d, %d, %d"), Primitive->IsCollisionEnabled(), Primitive->IsPhysicsCollisionEnabled(), 
	// Primitive->GetCollisionShape().IsSphere());


	// CurrentMode = UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetCharacterMovement()->GetGroundMovementMode();
}

// Called when the game starts or when spawned
void AEnderProjectile::BeginPlay()
{
	Super::BeginPlay();
	// CurrentMode = UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetCharacterMovement()->GetGroundMovementMode();

}

// Called every frame
void AEnderProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetActorLocation().Z < -1000)
	{
		// UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
		// Primitive->AddImpulse(FVector(0,0,1));
		UCharacterMovementComponent* Movement = UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetCharacterMovement();
		// Movement->SetMovementMode(EMovementMode::MOVE_Walking);
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *(Movement->GetAirControl().ToString()));
		Movement->SetMovementMode(CurrentMode);
		Movement->AirControl = 1.f;
		Movement->GravityScale = 0.5f;
		
		Destroy();
	}
}

// Called to bind functionality to input
void AEnderProjectile::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Function that initializes the projectile's velocity in the shoot direction.
void AEnderProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AEnderProjectile::ResetCamera()
{
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	Player->SetViewTargetWithBlend(Player, 0.2f);
}
// Function that is called when the projectile hits something.
/*void AEnderProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    // if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    // {
    //     OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
    // }
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
			Movement->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		Player->TeleportTo(CollisionLocation, Player->GetActorRotation());
	}
    Destroy();
}
*/