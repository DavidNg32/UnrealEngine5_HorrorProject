// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Game_HUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;
/**
 * 
 */
UCLASS()
class GAME_API AGame_HUD : public AHUD
{
	GENERATED_BODY()

public:
	//==============================================================================
	// PROPERTIES & VARIABLES
	//==============================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;

	//==============================================================================
	// FUNCTIONS
	//==============================================================================

	AGame_HUD();

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidgetText(const FInteractableData* InteractableData);

protected:
	

	//==============================================================================
	// PROPERTIES & VARIABLES
	//==============================================================================
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;
	
	//==============================================================================
	// FUNCTIONS
	//==============================================================================
	virtual void BeginPlay() override;
};
