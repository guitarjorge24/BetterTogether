// Copyright of Jorge Luque


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

	// GetActorLocation uses world space but TargetLocation is using local space because we set it through the
	// transform widget whose origin is this class's (the platform's) origin.
	// So we need to convert TargetLocation to worldspace.
	// TransformPosition() transforms the location passed as an arg by the transform's location that it's called from.
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Move platforms if there's enough triggers activated
	if (ActiveTriggers >= TriggersNeeded)
	{
		// Move the platform on the server only
		if (!HasAuthority()) { return; }

		FVector CurrentLocation = GetActorLocation();
		float JourneyDistance = (GlobalTargetLocation - GlobalStartLocation).Size();
		float DistanceTravelled = (CurrentLocation - GlobalStartLocation).Size();

		// Make the tigger move back to it's starting location once it reaches it's destination.
		if (DistanceTravelled > JourneyDistance)
		{
			Swap(GlobalStartLocation, GlobalTargetLocation);
		}

		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		CurrentLocation += Direction * Speed * DeltaSeconds;
		SetActorLocation(CurrentLocation);
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::SubtractActiveTrigger()
{
	ActiveTriggers--;
}
