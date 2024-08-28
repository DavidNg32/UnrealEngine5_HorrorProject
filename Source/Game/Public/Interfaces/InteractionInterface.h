// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AGameCharacter;

UENUM()
enum class EInteractableType : uint8
{
	IT_Use UMETA(DisplayName = "Use"),
	IT_Pickup UMETA(DisplayName = "Pickup"),
	IT_Talk UMETA(DisplayName = "Talk"),
	IT_Device UMETA(DisplayName = "Device"),
	IT_Toggle UMETA(DisplayName = "Toggle"),
	IT_None UMETA(DisplayName = "None")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()
	FInteractableData() :
	InteractableType(EInteractableType::IT_Pickup), InteractableName(FText::GetEmpty()),
	InteractableAction(FText::GetEmpty()),
	Quantity(0), InteractionDuration(0.0f)
	{};

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText InteractableName;

	UPROPERTY(EditInstanceOnly)
	FText InteractableAction;

	// only for pickups
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	//time it takes to interact (ie. hold to open door, turn valve, etc.)
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAME_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(AGameCharacter* PlayerCharacter);

	FInteractableData InteractableData;
};
