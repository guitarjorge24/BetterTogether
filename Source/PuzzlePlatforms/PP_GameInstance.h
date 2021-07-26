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
	UFUNCTION(Exec)
	void Host() override;
	UFUNCTION(Exec)
	void Join(const FString& IpAddress);

	private:
	TSubclassOf<class UUserWidget> MenuClass;
	UPROPERTY()
	class UMainMenu* Menu;
};
