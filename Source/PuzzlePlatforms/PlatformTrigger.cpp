// Copyright of Jorge Luque

#include "PlatformTrigger.h"
#include "MovingPlatform.h"
#include "Components/BoxComponent.h"

APlatformTrigger::APlatformTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));

	if (!ensure(TriggerVolume)) { return; }

	RootComponent = TriggerVolume;
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered Trigger Volume"))

	for (AMovingPlatform* Platform : PlatformsActivatedByTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Exited Trigger Volume"))
	for (AMovingPlatform* Platform : PlatformsActivatedByTrigger)
	{
		Platform->SubtractActiveTrigger();
	}
}