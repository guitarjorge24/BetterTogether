// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable); // the same as setting the mobility type in the details panel
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Since the platform is moved on the server only, we replicate the platform from the server to the client
	if (HasAuthority()) // Replication only goes one way so we make sure to set replication on the server only, not the client.
	{
		SetReplicates(true); // replicate the actor itself (e.g. spawning)
		SetReplicateMovement(true); // replicates the actor movement (e.g. position)
	}
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Move the platform on the server only
	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		Location += FVector(Speed * DeltaSeconds,0.f,0.f);
		SetActorLocation(Location);
	}

}
