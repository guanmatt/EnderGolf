// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "EnderPortal.h"

// Sets default values
AEnderPortal::AEnderPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PortalSceneComponent"));

	}
	CollisionBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionBody"));
	CollisionBody->SetupAttachment(RootComponent);
	CollisionBody->SetCollisionProfileName(TEXT("PortalCollision"));
	CollisionBody->SetCollisionObjectType(ECC_GameTraceChannel4);
	CollisionBody->OnComponentBeginOverlap.AddDynamic(this, &AEnderPortal::TeleportPortal);

	Elipse = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	Elipse->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/Meshes/SM_Portal.SM_Portal'"));
	if (Mesh.Succeeded())
	{
		Elipse->SetStaticMesh(Mesh.Object);
		Elipse->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Elipse->SetRelativeRotation(FRotator(0.f, 90.f,0.f));

	}
	// UE_LOG(LogTemp, Warning, TEXT("%d"), Elipse->IsCollisionEnabled());
	// Elipse->SetCollisionProfileName(TEXT("Portal"));
	//Elipse->SetCollisionObjectType(ECC_GameTraceChannel4);
	Elipse->SetCollisionProfileName(TEXT("NoCollision"));

	// Elipse->OnComponentHit.AddDynamic(this, &AEnderPortal::TeleportPortal2);
	//Elipse->OnComponentBeginOverlap.AddDynamic(this, &AEnderPortal::TeleportPortal);
}

// Called when the game starts or when spawned
void AEnderPortal::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("%s up is %s"), *GetActorLabel(), *GetActorUpVector().ToString());
	// Elipse->SetCollisionObjectType(ECC_GameTraceChannel4);
	// Elipse->SetCollisionProfileName(TEXT("PortalCollision"));
	// UE_LOG(LogTemp, Warning, TEXT("%s is facing %s"), *GetActorLabel(), *GetActorForwardVector().Rotation().ToString());
}

// Called every frame
void AEnderPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*TArray<AActor*> Overlap; 
	GetOverlappingActors(Overlap);
	if(Overlap.Num()>0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Overlap[0]->GetActorLabel())
		TArray<AActor*> FoundPortal;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnderPortal::StaticClass(), FoundPortal);
		for(int32 i = 0; i<FoundPortal.Num(); i++)
		{
			if(FoundPortal[i] != this && Cast<AEnderPortal>(FoundPortal[i])->PortalID == this->PortalID) //&& FoundActors[i]->PortalID == this->PortalID)
			{
				UE_LOG(LogTemp, Warning, TEXT("FOUND A MATCH: %s"), *FoundPortal[i]->GetActorLabel());
				UE_LOG(LogTemp, Warning, TEXT("info: %s"), *Overlap[0]->GetActorLabel() );
				UE_LOG(LogTemp, Warning, TEXT("forward: %s"), *FoundPortal[i]->GetActorForwardVector().ToString() );
				// Actor->GetActorLabel
				
				Overlap[0]->SetActorLocation(FoundPortal[i]->GetActorLocation() + FoundPortal[i]->GetActorUpVector() * 2,false);
				// Actor->GetRootComponent()->ComponentVelocity = FVector(0,0,0);
				break;
			}
		}
	}*/	
	// TArray<AActor*> Overlap;
	// GetOverlappingActors(Overlap);
	// if (Overlap.Num()>0)
	// {
	// 	for(int32 i = 0 ; i<Overlap.Num(); i++)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("%s"), *Overlap[i]->GetActorLabel() );

	// 	}
	// }
	/*FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FRotator NewRotation = Forward.Rotation();//GetActorRotation();


	NewRotation.Yaw += DeltaTime * 20.f;
	SetActorLocationAndRotation(Location, NewRotation);*/
}

void AEnderPortal::TeleportPortal(UPrimitiveComponent* OtherActor,AActor* Actor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("working") );
	TArray<AActor*> FoundPortal;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnderPortal::StaticClass(), FoundPortal);

	for(int32 i = 0; i<FoundPortal.Num(); i++)
	{
		
		if(FoundPortal[i] != this && Cast<AEnderPortal>(FoundPortal[i])->PortalID == this->PortalID) //&& FoundActors[i]->PortalID == this->PortalID)
		{
			UE_LOG(LogTemp, Warning, TEXT("FOUND A MATCH: %s"), *FoundPortal[i]->GetActorLabel());
	// 		// UE_LOG(LogTemp, Warning, TEXT("info: %s"), *Actor->GetActorLabel() );
	// 		// UE_LOG(LogTemp, Warning, TEXT("forward: %s"), *FoundPortal[i]->GetActorForwardVector().ToString() );
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetActorLabel());
			Actor->GetRootComponent()->ComponentVelocity=FVector(0,0,0);
			Actor->TeleportTo(FoundPortal[i]->GetActorLocation() + FoundPortal[i]->GetActorUpVector() * 166, Actor->GetActorRotation());
		
	// 		// Actor->GetRootComponent()->ComponentVelocity = FVector(0,0,0);
			break;
		}
	}
}

void AEnderPortal::TeleportPortal2(UPrimitiveComponent* OtherActor,AActor* Actor, UPrimitiveComponent* OtherComp, FVector Vector, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("working11: %s"), *this->GetActorLabel()  );
	
	TArray<AActor*> FoundPortal;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnderPortal::StaticClass(), FoundPortal);
	UE_LOG(LogTemp, Warning, TEXT("num: %f"), FoundPortal.Num());
	for(int32 i = 0; i<FoundPortal.Num(); i++)
	{
			UE_LOG(LogTemp, Warning, TEXT("FOUND A MATCH"));
		
		if(FoundPortal[i] != this && Cast<AEnderPortal>(FoundPortal[i])->PortalID == this->PortalID) //&& FoundActors[i]->PortalID == this->PortalID)
		{
		// 	UE_LOG(LogTemp, Warning, TEXT("info: %s"), *Actor->GetActorLabel() );
		// 	UE_LOG(LogTemp, Warning, TEXT("forward: %s"), *FoundPortal[i]->GetActorForwardVector().ToString() );
			
			Actor->SetActorLocation(FoundPortal[i]->GetActorLocation() + FoundPortal[i]->GetActorUpVector() * 2,false);
		// 	// Actor->GetRootComponent()->ComponentVelocity = FVector(0,0,0);
			break;
		}
	}
	/*for(int32 i = 0; i<FoundPortal.Num(); i++)
	{
		
		if(FoundPortal[i] != this && Cast<AEnderPortal>(FoundPortal[i])->PortalID == this->PortalID) //&& FoundActors[i]->PortalID == this->PortalID)
		{
			UE_LOG(LogTemp, Warning, TEXT("FOUND A MATCH: %s"), *FoundPortal[i]->GetActorLabel());
			UE_LOG(LogTemp, Warning, TEXT("info: %s"), *Actor->GetActorLabel() );
			UE_LOG(LogTemp, Warning, TEXT("forward: %s"), *FoundPortal[i]->GetActorForwardVector().ToString() );
			// Actor->GetActorLabel
			if (Actor->GetActorLabel() == "BP_BaseCharacter") {
				UE_LOG(LogTemp, Warning, TEXT("Player collision"));
				return;
			}
			Actor->SetActorLocation(FoundPortal[i]->GetActorLocation() + FVector(0,0,-5.0f),false);
			Actor->GetRootComponent()->ComponentVelocity = FVector(0,0,0);
			break;
		}
	}*/
	
}
