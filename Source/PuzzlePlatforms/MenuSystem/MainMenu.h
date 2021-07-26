// Copyright of Jorge Luque

#pragma once
#include "MenuInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* const InMenuInterface);
	/** @brief Sets up InputMode and cursor visibility */
	void SetupMenu();
	
protected:
	virtual bool Initialize() override;
	/** @brief Used to undo the stuff done by SetupMenu() */
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	
private:
	IMenuInterface* MenuInterface;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void HostServer();
	UFUNCTION()
	void JoinServer();
};
