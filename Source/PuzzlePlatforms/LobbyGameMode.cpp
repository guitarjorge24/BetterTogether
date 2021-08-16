// Copyright of Jorge Luque

#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++PlayerCount;
	
	UE_LOG(LogTemp, Warning, TEXT("PostLogin called for a player %s"), *NewPlayer->GetName())
	if (PlayerCount == MaxPlayersNum)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached %d players (the max num of players)."), PlayerCount)
		
		UWorld* World = GetWorld();
		if (!ensure(World)) { return; }
		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/Game?listen"); // the ?listen parameter makes this client a listen server
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	--PlayerCount;
}
