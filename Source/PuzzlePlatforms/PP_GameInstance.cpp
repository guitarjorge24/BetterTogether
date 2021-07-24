// Copyright of Jorge Luque

#include "PP_GameInstance.h"

#include "PlatformTrigger.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

UPP_GameInstance::UPP_GameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor was called"));

	ConstructorHelpers::FClassFinder<UUserWidget> MenuWBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuWBPClass.Class)) { return; }
	MenuClass = MenuWBPClass.Class;
}

void UPP_GameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init was called"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UPP_GameInstance::LoadMenu()
{
	UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);
	if (!ensure(Menu)) { return; }
	Menu->AddToViewport();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) { return; };

	FInputModeUIOnly InputModeData;
	// bShowMouseCursor only makes the cursor show up but if you play a standalone version of the game, the focus will not be on the menu widget by
	// default which means that even when the cursor is hovering over the button, it won't interact with it unless you click on a button or UI first
	// to change the focus to the widget. SetWidgetToFocus allows us to change that focus manually.
	InputModeData.SetWidgetToFocus(Menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen); // Cursor can't go off the game's window in fullscreen mode.
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UPP_GameInstance::Host()
{
	if (ensure(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Host Console Command Called!"));
	}

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	World->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen"); // the ?listen parameter makes this client a listen server
}

void UPP_GameInstance::Join(const FString& IpAddress)
{
	if (ensure(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Joined %s"), *IpAddress));
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) { return; }

	PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
}
