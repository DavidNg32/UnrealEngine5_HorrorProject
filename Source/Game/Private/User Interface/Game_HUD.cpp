// Fill out your copyright notice in the Description page of Project Settings.


#include "User Interface/Game_HUD.h"

#include "Chaos/ChaosPerfTest.h"
#include "User Interface/MainMenu.h"
#include "User Interface/Interaction/InteractionWidget.h"

AGame_HUD::AGame_HUD()
{
}

void AGame_HUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AGame_HUD::DisplayMenu()
{
	if(MainMenuWidget)
	{
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
		bIsMenuVisible = true;
	}
}

void AGame_HUD::HideMenu()
{
	if(MainMenuWidget)
	{
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		bIsMenuVisible = false;
	}
}

void AGame_HUD::ShowInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void AGame_HUD::HideInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AGame_HUD::UpdateInteractionWidgetText(const FInteractableData* InteractableData)
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			ShowInteractionWidget();
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}


