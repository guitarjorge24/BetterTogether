// Copyright of Jorge Luque

#include "ServerRow.h"

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UServerRow::SetParentAndIndex(UMainMenu* InParentMainMenu, uint32 InIndex)
{
	ParentMainMenu = InParentMainMenu;
	Index = InIndex;
	ServerRowButton->OnClicked.AddDynamic(this, &UServerRow::OnServerRowButtonClicked);
	
}

void UServerRow::OnServerRowButtonClicked()
{
	ParentMainMenu->SelectIndex(Index);
}
