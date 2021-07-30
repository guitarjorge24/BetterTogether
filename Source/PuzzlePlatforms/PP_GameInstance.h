// Copyright of Jorge Luque

#pragma once
#include "MenuSystem/MenuInterface.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
	void LoadMenu();
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();
	UFUNCTION(Exec)
	virtual void Host() override;
	UFUNCTION(Exec)
	virtual void Join(const FString& IpAddress) override;

	private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY()
	class UMainMenu* Menu;
};
