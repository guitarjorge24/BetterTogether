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

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta=(BindWidget))
	UButton* LANHostButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ToJoinIPMenuButton;
	UPROPERTY(meta=(BindWidget))
	UButton* JoinIPButton;

	UPROPERTY(meta=(BindWidget))
	UButton* SteamHostButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ToJoinSteamMenuButton;
	UPROPERTY(meta=(BindWidget))

	UButton* BackToMatchModesButton;
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta=(BindWidget))
	UWidget* MatchModesMenuOverlay;
	UPROPERTY(meta=(BindWidget))
	UWidget* JoinMenuOverlay;
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* IPTextBox;

	UPROPERTY()
	UWidget* PreviousWidget;

	UFUNCTION()
	void OnLANHostButtonClicked();
	UFUNCTION()
	void OnJoinIPMenuButtonClicked();
	UFUNCTION()
	void OnSteamHostButtonClicked();
	UFUNCTION()
	void OnJoinIPButtonClicked();
	UFUNCTION()
	void SwitchToPreviousMenu();
	UFUNCTION()
	void QuitButtonClicked();
};
