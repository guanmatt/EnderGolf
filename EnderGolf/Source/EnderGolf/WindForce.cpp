// Fill out your copyright notice in the Description page of Project Settings.

#include "WindForce.h"
#include "GameFramework/Actor.h"

// Sets default values
AWindForce::AWindForce()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWindForce::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWindForce::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor *> Overlapping;
	GetOverlappingActors(Overlapping);
	if (Overlapping.Num() > 0)
	{
		for (int32 i = 0; i<Overlapping.Num(); i++)
		{
			// UE_LOG(LogTemp, Warning, TEXT("overlap detected")  );
		}
	}
}

bool AWindForce::IsActorOverlapping()
{
	return false;
}
