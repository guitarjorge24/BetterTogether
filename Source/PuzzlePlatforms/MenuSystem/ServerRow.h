// Copyright of Jorge Luque

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ServerName;

	/**
 	* @brief Sets the parent menu and row index. 
 	* @param InParentMainMenu The main menu widget that contains the scrollbox upon which the ServerRows are added to as children
 	* @param InIndex The server row's index
 	*/
	void SetParentAndIndex(class UMainMenu* InParentMainMenu, uint32 InIndex);

private:
	UPROPERTY(meta=(BindWidget))
	class UButton* ServerRowButton;
	UPROPERTY()
	class UMainMenu* ParentMainMenu;
	uint32 Index;

	UFUNCTION()
	void OnServerRowButtonClicked();
};
