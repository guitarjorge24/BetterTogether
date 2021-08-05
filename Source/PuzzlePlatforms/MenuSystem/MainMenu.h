// Copyright of Jorge Luque

#pragma once

#include "MenuBase.h"
#include "CoreMinimal.h"

#include "MainMenu.generated.h"

class UButton;
class UWidget;
class UWidgetSwitcher;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

public:
	UMainMenu();
protected:
	virtual bool Initialize() override;

private:
	TSubclassOf<class UUserWidget> ServerRowClass;
	
	// LAN Host/Join Buttons
	UPROPERTY(meta=(BindWidget))
	UButton* LANHostButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ToJoinIPMenuButton;
	UPROPERTY(meta=(BindWidget))
	UButton* JoinIPButton;

	// Steam Host/Join Buttons
	UPROPERTY(meta=(BindWidget))
	UButton* SteamHostButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ToJoinSteamMenuButton;
	UPROPERTY(meta=(BindWidget))
	UButton* JoinSteamSessionButton;

	// Back/Quit Buttons
	UPROPERTY(meta=(BindWidget))
	UButton* BackButtonInJoinIPMenu;
	UPROPERTY(meta=(BindWidget))
	UButton* BackButtonInSteamJoinMenu;
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

	// Main Menu Widgets
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta=(BindWidget))
	UWidget* MatchModesMenuOverlay;
	UPROPERTY(meta=(BindWidget))
	UWidget* JoinIPMenuOverlay;
	UPROPERTY(meta=(BindWidget))
	UWidget* JoinSteamMenuOverlay;
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* IPTextBox;
	UPROPERTY(meta=(BindWidget))
	UPanelWidget* ServerListScrollBox;

	UPROPERTY()
	UWidget* PreviousWidget;

	UFUNCTION()
	void OnLANHostButtonClicked();
	/** Open Join Menu where you type the server IP */
	UFUNCTION()
	void OnJoinIPMenuButtonClicked();
	UFUNCTION()
	void OnSteamHostButtonClicked();
	UFUNCTION()
	void OnJoinSteamMenuButtonClicked();


	UFUNCTION()
	void OnJoinIPButtonClicked();
	UFUNCTION()
	void OnJoinSteamSessionButtonClicked();
	UFUNCTION()
	void SwitchToPreviousMenu();
	UFUNCTION()
	void QuitButtonClicked();
};
