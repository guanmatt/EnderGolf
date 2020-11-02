// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Blueprint.h"
#include "EnderProjectile.h"

// Sets default values
AEnderProjectile::AEnderProjectile()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    // SpringArm->SetupAttachment(RootComponent);
    // Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    // Camera->SetupAttachment(SpringArm);
	
	// ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	// if(!RootComponent)
	// {
	// 	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	// }

	if(!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if(!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = 1500.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
	}
	// if(!ProjectileMeshComponent)
	// {
	// 	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	// 	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/Sphere.Sphere'"));
	// 	if(Mesh.Succeeded())
	// 	{
	// 		ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
	// 	}
	// }
	// static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Fbx_Default_Material.Fbx_Default_Material'"));
	// if (Material.Succeeded())
	// {
	// 	ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	// }
	// ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	// ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	// ProjectileMeshComponent->SetupAttachment(RootComponent);


	// Set the sphere's collision profile name to "Projectile".
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	// Event called when component hits something.
	CollisionComponent->OnComponentHit.AddDynamic(this, &AEnderProjectile::OnHit);


}

// Called when the game starts or when spawned
void AEnderProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnderProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

// Function that is called when the projectile hits something.
void AEnderProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
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
		Player->TeleportTo(CollisionLocation, Player->GetActorRotation());
	}
    Destroy();
}
