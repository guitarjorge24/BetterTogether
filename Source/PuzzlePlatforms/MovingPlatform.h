// Copyright of Jorge Luque

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * Class for the moving platforms used in the puzzle. They require to have at least 1 active trigger to move.
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
#pragma region Setup + Tick
public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

#pragma endregion
	
public:
	UPROPERTY(EditAnywhere)
	float Speed = 20.f;
	UPROPERTY(EditAnywhere, Category="!Moving Platform", meta=(MakeEditWidget="true"))
	FVector TargetLocation;

	void AddActiveTrigger();
	void SubtractActiveTrigger();

private:
	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;
	/**
	* @brief The number of triggers that need to be activated for the platform to start moving.
	* Leave at 0 for platforms that should always move and dont require triggers.
	*/
	UPROPERTY(EditAnywhere)
	uint8 TriggersNeeded = 0;

	/**
 	* @brief The number of triggers currently activating this platform 
 	*/
	UPROPERTY(EditAnywhere)
	uint8 ActiveTriggers = 0;
};
