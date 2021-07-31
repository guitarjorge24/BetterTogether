// Copyright of Jorge Luque

#pragma once

#include "MenuBase.h"

#include "CoreMinimal.h"

#include "InGameMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuBase
{
	GENERATED_BODY()

public:
	void SetupInGameMenuInputComponent();

protected:
	virtual bool Initialize() override;
	virtual void RemoveMenu() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void QuitPressed();

	UPROPERTY()
	UInputComponent* InGameMenuInputComponent;
};
