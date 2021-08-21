// Copyright of Jorge Luque

#include "LobbyGameMode.h"

#include "PP_GameInstance.h"
#include "TimerManager.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++PlayerCount;
	
	UE_LOG(LogTemp, Warning, TEXT("PostLogin called for a player %s"), *NewPlayer->GetName())
	if (PlayerCount == MaxPlayersNum - 1) // one more person can join but the timer will start counting once you have max - 1 players
	{
		GetWorldTimerManager().SetTimer(StartMatchTimerHandle, this, &ALobbyGameMode::StartGame, 10.f, false);
		UE_LOG(LogTemp, Warning, TEXT("Reached %d players (the max num of players)."), PlayerCount)
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	--PlayerCount;
}

void ALobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UPP_GameInstance>(GetGameInstance());
	if (ensure(GameInstance))
	{
		GameInstance->StartSession();
	}
	
	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Game?listen"); // the ?listen parameter makes this client a listen server
}
