// Copyright of Jorge Luque

#pragma once
#include "MenuSystem/MenuInterface.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "PP_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPP_GameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPP_GameInstance();
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void CreateMainMenuWidget();
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	virtual void HostLANServer() override;
	UFUNCTION(Exec)
	virtual void JoinLANServer(const FString& IpAddress) override;
	/** Called when SteamHostButton is clicked. Creates new session. If session already exists, destroys it which triggers OnDestroySessionComplete() */
	UFUNCTION(Exec)
	virtual void HostSteamServer() override;
	UFUNCTION(Exec)
	virtual void JoinSteamServer(uint32 Index) override;
	virtual void LoadMainMenuMap() override;

	virtual void RefreshServerList() override;

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY()
	class UMainMenu* Menu;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UInGameMenu* InGameMenu;
	IOnlineSessionPtr SessionInterface; // can't be forward declared because IOnlineSessionPtr is a typedef defined in OnlineSessionInterface.h

	// Old method of creating the TSharedRef variable. I switched to creating a TSharedPtr and then converting to Ref when needed with ToSharedRef().
	// TSharedRef<FOnlineSessionSearch> can't be forward declared because the default constructor for TSharedRef<> does "new".
	// Also you can't use the default constructor to create a blank object so you have to initialize it upon declaring.
	// TSharedRef<FOnlineSessionSearch> SessionSearch = MakeShared<FOnlineSessionSearch>(); // MakeShared creates a new object in heap;
	
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/** Delegate fired after completed session create request in UPP_GameInstance::HostSteamServer()
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	/** Delegate fired when a destroying an online session has completed. If destroying was successful, create a new session. */
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSession();
};
