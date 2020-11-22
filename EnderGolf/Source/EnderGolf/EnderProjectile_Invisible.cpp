// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "EnderProjectile_Invisible.h"

AEnderProjectile_Invisible::AEnderProjectile_Invisible()
{

    // if(CollisionComponent)
    // {
    //     CollisionComponent->DestroyComponent();
    // }
    if(ProjectileMeshComponent)
    {
        ProjectileMeshComponent->DestroyComponent();
    }
    GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnderProjectile_Invisible::DrawTrajectory, 1.0f, true);
}

void AEnderProjectile_Invisible::Tick(float DeltaTime)
{
    // DrawDebugPoint(GetWorld(), this->GetActorLocation(), 15.f, FColor(255,255,255), false, 10.f);
}

void AEnderProjectile_Invisible::DrawTrajectory()
{
    DrawDebugPoint(GetWorld(), this->GetActorLocation(), 15.f, FColor(255,255,255), false, 10.f);
}

void AEnderProjectile_Invisible::BeginPlay()
{
	Super::BeginPlay();

    
}