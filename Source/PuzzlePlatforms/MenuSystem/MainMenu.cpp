// Copyright of Jorge Luque

#include "MainMenu.h"

#include "Chaos/ChaosPerfTest.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"

bool UMainMenu::Initialize()
{
	if (!ensure(Super::Initialize())) { return false; } // Return if base class failed to initialize

	if (!ensure(HostButton)) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	
	if(!ensure(JoinIPButton)) {return false;}
	JoinIPButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(ToJoinMenuButton)) { return false; }
	ToJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(BackToMatchModesButton)) { return false; }
	BackToMatchModesButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchToPreviousMenu);

	return true;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Host button pressed"))
	if (!ensure(MenuInterface)) { return; }
	MenuInterface->Host();
}

void UMainMenu::OpenJoinMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Join button pressed"))
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(JoinMenuOverlay)) { return; }
	PreviousWidget = MatchModesMenuOverlay;
	MenuSwitcher->SetActiveWidget(JoinMenuOverlay);
}

void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Join button on IP Address Menu pressed"))
	if (!ensure(MenuInterface)) { return; }
	MenuInterface->Join(IPTextBox->GetText().ToString());
}

void UMainMenu::SwitchToPreviousMenu()
{
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(PreviousWidget)) { return; }
	MenuSwitcher->SetActiveWidget(PreviousWidget);
}
