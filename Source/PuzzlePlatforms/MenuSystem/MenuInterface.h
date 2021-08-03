// Copyright of Jorge Luque

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUZZLEPLATFORMS_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HostLANServer() = 0;
	virtual void JoinLANServer(const FString& IpAddress) = 0;
	virtual void HostSteamServer() = 0;
	
	virtual void LoadMainMenuMap() = 0;
};
