// Copyright of Jorge Luque

#pragma once

#include "MenuInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuBase.generated.h"

class IMenuInterface;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* const InMenuInterface);
	/** @brief Sets up InputMode and cursor visibility */
	void SetupMenu();


protected:
	IMenuInterface* MenuInterface;

	/** @brief Used to undo the stuff done by SetupMenu() */
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
