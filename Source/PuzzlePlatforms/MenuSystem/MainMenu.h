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
	UButton* HostButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ToJoinMenuButton;
	UPROPERTY(meta=(BindWidget))
	UButton* JoinIPButton;
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
	void HostServer();
	UFUNCTION()
	void OpenJoinMenu();
	UFUNCTION()
	void JoinServer();
	UFUNCTION()
	void SwitchToPreviousMenu();
	UFUNCTION()
	void QuitButtonPressed();
	
};
