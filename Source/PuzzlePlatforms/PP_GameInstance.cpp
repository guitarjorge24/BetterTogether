// Copyright of Jorge Luque

#include "PP_GameInstance.h"
#include "MenuSystem/MenuBase.h"
#include "MenuSystem/MainMenu.h"

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
	
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuWBPClass.Class)) { return; }
	InGameMenuClass = InGameMenuWBPClass.Class;
}

void UPP_GameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init was called"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UPP_GameInstance::LoadMenu()
{
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu)) { return; }
	Menu->SetupMenu();
	Menu->SetMenuInterface(this);
}

void UPP_GameInstance::LoadInGameMenu()
{
	UMenuBase* InGameMenu = CreateWidget<UMenuBase>(this, InGameMenuClass);
	if (!ensure(InGameMenu)) { return; }
	InGameMenu->SetupMenu();
	InGameMenu->SetMenuInterface(this);	
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
