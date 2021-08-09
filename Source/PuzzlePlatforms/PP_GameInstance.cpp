// Copyright of Jorge Luque

#include "PP_GameInstance.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MainMenu.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

const static FName k_SessionName("Steam Session");

UPP_GameInstance::UPP_GameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor was called"));

	ConstructorHelpers::FClassFinder<UUserWidget> MenuWBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuWBPClass.Class)) { return; }
	MainMenuClass = MenuWBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuWBPClass.Class)) { return; }
	InGameMenuClass = InGameMenuWBPClass.Class;
}

void UPP_GameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init was called"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());

	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	if (OSS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found OSS named: %s"), *OSS->GetSubsystemName().ToString());
		SessionInterface = OSS->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			// If the class that contains the function that's being added to the delegate is a UObject class, then you would use AddUObject().
			// UPP_GameInstance inherits from UGameInstance which inherits from UObject.
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPP_GameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPP_GameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPP_GameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPP_GameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OSS was NULL"));
	}
}

void UPP_GameInstance::CreateMainMenuWidget()
{
	Menu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (!ensure(Menu)) { return; }
	Menu->SetupMenu();
	Menu->SetMenuInterface(this);
}

void UPP_GameInstance::LoadInGameMenu()
{
	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (!ensure(InGameMenu)) { return; }
	InGameMenu->SetupMenu();
	InGameMenu->SetMenuInterface(this);
	InGameMenu->SetupInGameMenuInputComponent();
}

void UPP_GameInstance::HostLANServer()
{
	if (ensure(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Host Console Command Called!"));
	}

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	World->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen"); // the ?listen parameter makes this client a listen server
}

void UPP_GameInstance::JoinLANServer(const FString& IpAddress)
{
	if (ensure(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Joined %s"), *IpAddress));
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) { return; }

	PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
}

void UPP_GameInstance::HostSteamServer()
{
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(k_SessionName);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(k_SessionName);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPP_GameInstance::JoinSteamServer(uint32 Index)
{
	if (!ensure(SessionInterface.IsValid()) || !ensure(Menu) || !ensure(SessionSearch.IsValid())) { return; }
	// Is it necessary to call Menu->RemoveMenu() ? It might already be called through OnLevelRemovedFromWorld when we try to join a session.
	// Menu->RemoveMenu();

	SessionInterface->JoinSession(0, k_SessionName, SessionSearch->SearchResults[Index]);
}

void UPP_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session creation failed."));
		return;
	}
	if (ensure(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Session Creation Successful!"));
	}

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	World->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen"); // the ?listen parameter makes this client a listen server
}

void UPP_GameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		CreateSession();
	}
}

void UPP_GameInstance::CreateSession()
{
	if (SessionInterface)
	{
		FOnlineSessionSettings SessionSettings;

		// Set bIsLANMatch to false when using the Steam OSS but set to true when testing with the NULL OSS which can only do LAN matches.
		SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
		SessionSettings.NumPublicConnections = 2; // Sets the max player number in the session
		SessionSettings.bShouldAdvertise = true; // publicly advertise the match on the server
		SessionSettings.bUsesPresence = true; // Required for Steam Lobby
		SessionInterface->CreateSession(0, k_SessionName, SessionSettings);
	}

	// OnCreateSessionComplete() fires after this.
}

void UPP_GameInstance::LoadMainMenuMap()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) { return; }

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPP_GameInstance::RefreshServerList()
{
	SessionSearch = MakeShared<FOnlineSessionSearch>();

	if (SessionSearch.IsValid() && SessionInterface.IsValid())
	{
		// Even on LAN matches this is not necessary since not setting this to true just means it will search for both LAN and non LAN sessions.
		// SessionSearch->bIsLanQuery = true;

		// This adds another search rule/parameter to the query.
		// In this search, only the sessions that have a SEARCH_PRESENCE that is Equals to true will show up in the search results.
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Type::Equals);
		SessionSearch->MaxSearchResults = 200;

		UE_LOG(LogTemp, Warning, TEXT("FindSessions search started at %f seconds."), GetWorld()->TimeSeconds)
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPP_GameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("FindSessions search completed at %f seconds with a status of %d"), GetWorld()->TimeSeconds, bWasSuccessful);

	if (bWasSuccessful && SessionSearch.IsValid())
	{
		TArray<FServerData> ServerDataList;

		// #Debug: Used for previewing what server entries look like without needing to host multiple sessions.
		// ServerNamesList.Add("Test Server 1");
		// ServerNamesList.Add("Test Server 2");
		// ServerNamesList.Add("Test Server 3");

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch.Get()->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session with ID of: %s"), *SearchResult.GetSessionIdStr())

			FServerData ServerData;
			ServerData.Name = SearchResult.GetSessionIdStr();
			ServerData.MaxNumberOfPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			// NumOpenPublicConnections is the num of available connections left in the session
			ServerData.CurrentPlayersCount = ServerData.MaxNumberOfPlayers - SearchResult.Session.NumOpenPublicConnections;
			ServerData.HostUserName = SearchResult.Session.OwningUserName;
			
			ServerDataList.Add(ServerData);

		}

		Menu->SetServerList(ServerDataList);
	}
}

void UPP_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString ConnectInfo; //The URL passed into ClientTravel
	if (SessionInterface->GetResolvedConnectString(k_SessionName, ConnectInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("ConnectInfo: %s"), *ConnectInfo)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ConnectInfo: %s"), *ConnectInfo));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get ConnectInfo string"))
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) { return; }
	PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
}
