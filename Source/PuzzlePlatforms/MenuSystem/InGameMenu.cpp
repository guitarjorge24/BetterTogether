// Copyright of Jorge Luque

#include "InGameMenu.h"

#include "Components/Button.h"

void UInGameMenu::SetupInGameMenuInputComponent()
{
	InGameMenuInputComponent =  NewObject<UInputComponent>(GetOwningPlayer()); //Makes the player controller the owner of the InputComponent
	if (InGameMenuInputComponent)
	{
		InGameMenuInputComponent->RegisterComponent();
		InGameMenuInputComponent->bBlockInput = true;
		InGameMenuInputComponent->BindAction(TEXT("OpenGameMenu"), IE_Pressed, this, &UInGameMenu::RemoveMenu);
		GetOwningPlayer()->PushInputComponent(InGameMenuInputComponent);
	}
}

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize()) { return false; } // Return if base class failed to initialize

	if (!ensure(CancelButton)) { return false; }
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::RemoveMenu);

	if (!ensure(QuitButton)) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	return true;
}

void UInGameMenu::RemoveMenu()
{
	Super::RemoveMenu();
	GetOwningPlayer()->PopInputComponent(InGameMenuInputComponent);
}

void UInGameMenu::QuitPressed()
{
	if (!ensure(MenuInterface)) { return; }
	RemoveMenu();
	MenuInterface->LoadMainMenuMap();
}
