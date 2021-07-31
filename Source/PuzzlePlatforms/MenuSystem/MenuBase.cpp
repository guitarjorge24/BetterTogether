// Copyright of Jorge Luque

#include "MenuBase.h"

#include <string>

void UMenuBase::SetMenuInterface(IMenuInterface* const InMenuInterface)
{
	MenuInterface = InMenuInterface;
}

void UMenuBase::SetupMenu()
{
	this->AddToViewport();

	APlayerController* PlayerController = GetOwningPlayer();

	if (!ensure(PlayerController)) { return; };

	FInputModeGameAndUI InputModeData; // Input is only used/detected by UI
	// bShowMouseCursor only makes the cursor show up but if you play a standalone version of the game, the focus will not be on the menu widget by
	// default which means that even when the cursor is hovering over the button, it won't interact with it unless you click on a button or UI first
	// to change the focus to the widget. SetWidgetToFocus allows us to change that focus manually.
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen); // Cursor can't go off the game's window in fullscreen mode.
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMenuBase::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	RemoveMenu();
}

void UMenuBase::RemoveMenu()
{
	this->RemoveFromViewport(); // Not necessary when the world gets destroyed upon server travel but still needed when wanting to remove a menu mid game.
	APlayerController* PlayerController = GetOwningPlayer();
	FInputModeGameOnly InputModeData; // Input is used/detected by player controller
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}