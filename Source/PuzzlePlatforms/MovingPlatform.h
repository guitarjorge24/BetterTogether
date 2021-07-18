// Copyright of Jorge Luque

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

	UPROPERTY(EditAnywhere)
	float Speed = 20.f;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget="true"))
	FVector TargetLocation;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	private:
	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;
};
