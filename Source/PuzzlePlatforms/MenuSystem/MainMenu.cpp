// Copyright of Jorge Luque

#include "MainMenu.h"

#include "Chaos/ChaosPerfTest.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"

bool UMainMenu::Initialize()
{
	if (!ensure(Super::Initialize())) { return false; } // Return if base class failed to initialize

	if (!ensure(LANHostButton)) { return false; }
	LANHostButton->OnClicked.AddDynamic(this, &UMainMenu::OnLANHostButtonClicked);
	if (!ensure(ToJoinIPMenuButton)) { return false; }
	ToJoinIPMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinIPMenuButtonClicked);
	if (!ensure(SteamHostButton)) { return false; }
	SteamHostButton->OnClicked.AddDynamic(this, &UMainMenu::OnSteamHostButtonClicked);

	if (!ensure(QuitButton)) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitButtonClicked);

	if (!ensure(BackToMatchModesButton)) { return false; }
	BackToMatchModesButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchToPreviousMenu);
	if (!ensure(JoinIPButton)) { return false; }
	JoinIPButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinIPButtonClicked);

	return true;
}

void UMainMenu::OnLANHostButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("LAN Host button pressed"))
	if (!ensure(MenuInterface)) { return; }
	MenuInterface->HostLANServer();
}

void UMainMenu::OnJoinIPMenuButtonClicked()
{
	// Open Join Menu
	UE_LOG(LogTemp, Warning, TEXT("LAN Join button pressed"))
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(JoinMenuOverlay)) { return; }
	PreviousWidget = MatchModesMenuOverlay;
	MenuSwitcher->SetActiveWidget(JoinMenuOverlay);
}

void UMainMenu::OnSteamHostButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Steam Host button pressed"))
	if (!ensure(MenuInterface)) { return; }
	MenuInterface->HostSteamServer();
}

void UMainMenu::OnJoinIPButtonClicked()
{
	// Join LAN Server
	UE_LOG(LogTemp, Warning, TEXT("Join button on IP Address Menu pressed"))
	if (!ensure(MenuInterface)) { return; }
	MenuInterface->JoinLANServer(IPTextBox->GetText().ToString());
}

void UMainMenu::SwitchToPreviousMenu()
{
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(PreviousWidget)) { return; }
	MenuSwitcher->SetActiveWidget(PreviousWidget);
}

void UMainMenu::QuitButtonClicked()
{
	GetOwningPlayer()->ConsoleCommand(TEXT("quit"));
}
