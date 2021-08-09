// Copyright of Jorge Luque

#include "MainMenu.h"
#include "ServerRow.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/ConstructorHelpers.h"

UMainMenu::UMainMenu()
{
	//Somehow, even if I dont explicitly call the base class's constructor "UUserWidget(const FObjectInitializer& ObjectInitializer)" in my
	// no-arg constructor initialization list, the base class's 1-arg constructor still gets called right before this constructor executes.
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowWBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowWBPClass.Class)) { return; }
	ServerRowClass = ServerRowWBPClass.Class;
}

bool UMainMenu::Initialize()
{
	if (!ensure(Super::Initialize())) { return false; } // Return if base class failed to initialize

	// Main Menu
	if (!ensure(LANHostButton)) { return false; }
	LANHostButton->OnClicked.AddDynamic(this, &UMainMenu::OnLANHostButtonClicked);
	if (!ensure(ToJoinIPMenuButton)) { return false; }
	ToJoinIPMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinIPMenuButtonClicked);
	if (!ensure(SteamHostButton)) { return false; }
	SteamHostButton->OnClicked.AddDynamic(this, &UMainMenu::OnSteamHostButtonClicked);
	if (!ensure(ToJoinSteamMenuButton)) { return false; }
	ToJoinSteamMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinSteamMenuButtonClicked);

	if (!ensure(QuitButton)) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitButtonClicked);

	// Join IP Menu
	if (!ensure(BackButtonInJoinIPMenu)) { return false; }
	BackButtonInJoinIPMenu->OnClicked.AddDynamic(this, &UMainMenu::SwitchToPreviousMenu);
	if (!ensure(JoinIPButton)) { return false; }
	JoinIPButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinIPButtonClicked);

	// Join Steam Session Menu
	if (!ensure(BackButtonInSteamJoinMenu)) { return false; }
	BackButtonInSteamJoinMenu->OnClicked.AddDynamic(this, &UMainMenu::SwitchToPreviousMenu);
	if (!ensure(JoinSteamSessionButton)) { return false; }
	JoinSteamSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinSteamSessionButtonClicked);

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
	if (!ensure(JoinIPMenuOverlay)) { return; }
	PreviousWidget = MatchModesMenuOverlay;
	MenuSwitcher->SetActiveWidget(JoinIPMenuOverlay);
}

void UMainMenu::OnSteamHostButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Steam Host button pressed"))
	if (!ensure(MenuInterface)) { return; }
	MenuInterface->HostSteamServer();
}

void UMainMenu::OnJoinSteamMenuButtonClicked()
{
	// Open menu with list of steam sessions
	UE_LOG(LogTemp, Warning, TEXT("Steam Join button pressed"))
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(JoinSteamMenuOverlay)) { return; }
	PreviousWidget = MatchModesMenuOverlay;
	MenuSwitcher->SetActiveWidget(JoinSteamMenuOverlay);
	if (!ensure(MenuInterface)) { return; }
	MenuInterface->RefreshServerList();
}

void UMainMenu::SetServerList(TArray<FServerData> ServerDataList)
{
	ServerListScrollBox->ClearChildren();

	uint32 ServerNamesIndex = 0;
	for (const FServerData& ServerData : ServerDataList)
	{
		UServerRow* ServerRowWidget = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(ServerRowWidget)) { return; }

		ServerRowWidget->ServerName->SetText(FText::FromString(ServerData.Name));
		ServerRowWidget->HostUserName->SetText(FText::FromString(ServerData.HostUserName));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayersCount, ServerData.MaxNumberOfPlayers);
		ServerRowWidget->NumberOfPlayersFraction->SetText(FText::FromString(FractionText));
		ServerRowWidget->SetParentAndIndex(this, ServerNamesIndex);
		++ServerNamesIndex;

		if (!ensure(ServerListScrollBox)) { return; }
		ServerListScrollBox->AddChild(ServerRowWidget);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	UpdateChildren(SelectedIndex, Index);
	SelectedIndex = Index;
}

void UMainMenu::UpdateChildren(TOptional<uint32> PreviousIndexOptional, int32 CurrentIndex)
{
	if (!PreviousIndexOptional.IsSet())
	{
		auto CurrentRow = Cast<UServerRow>(ServerListScrollBox->GetChildAt(CurrentIndex));
		CurrentRow->bIsSelected = true;
		return;
	}

	if (PreviousIndexOptional == CurrentIndex) { return; }

	auto PreviousRow = Cast<UServerRow>(ServerListScrollBox->GetChildAt(PreviousIndexOptional.GetValue()));
	PreviousRow->bIsSelected = false;

	auto CurrentRow = Cast<UServerRow>(ServerListScrollBox->GetChildAt(CurrentIndex));
	CurrentRow->bIsSelected = true;
}


void UMainMenu::OnJoinIPButtonClicked()
{
	// Join LAN Server
	UE_LOG(LogTemp, Warning, TEXT("Join button on IP Address Menu pressed"))
	if (!ensure(MenuInterface)) { return; }
	MenuInterface->JoinLANServer(IPTextBox->GetText().ToString());
}

void UMainMenu::OnJoinSteamSessionButtonClicked()
{
	// Join Steam Server (for now it joins LAN server until we have figured out Steam joining)
	UE_LOG(LogTemp, Warning, TEXT("Join button on Steam Server List Menu pressed"))

	if (SelectedIndex.IsSet() && ensure(MenuInterface))
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex: %d"), SelectedIndex.GetValue())
		MenuInterface->JoinSteamServer(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex not set"))
	}
}

void UMainMenu::SwitchToPreviousMenu()
{
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(PreviousWidget)) { return; }
	MenuSwitcher->SetActiveWidget(PreviousWidget);
}

void UMainMenu::OnQuitButtonClicked()
{
	GetOwningPlayer()->ConsoleCommand(TEXT("quit"));
}
