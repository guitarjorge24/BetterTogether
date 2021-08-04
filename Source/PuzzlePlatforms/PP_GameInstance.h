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
	virtual void LoadMainMenuMap() override;

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY()
	class UMainMenu* Menu;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UInGameMenu* InGameMenu;
	IOnlineSessionPtr SessionInterface; // can't be forward declared because IOnlineSessionPtr is a typedef defined in OnlineSessionInterface.h
	
	/**
	* Delegate fired after completed session create request in UPP_GameInstance::HostSteamServer()
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnCreateSessionComplete(FName SessionName,bool bWasSuccessful);
	/** Delegate fired when a destroying an online session has completed. If destroying was successful, create a new session. */
	void OnDestroySessionComplete(FName SessionName,bool bWasSuccessful);
	void CreateSession();
};
