// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GameCharacter.generated.h"

class AGame_HUD;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData(): CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f) {};
	
	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

class IInteractionInterface;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	//==============================================================================
	// PROPERTIES & VARIABLES
	//==============================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	//==============================================================================
	// FUNCTIONS
	//==============================================================================

	AGameCharacter();
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(InteractionCheckTimerHandle); }

protected:
	//==============================================================================
	// PROPERTIES & VARIABLES
	//==============================================================================

	UPROPERTY()
	AGame_HUD* HUD;

	UPROPERTY(VisibleAnywhere, Category= "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequency;
	float InteractionCheckDistance;

	FTimerHandle InteractionCheckTimerHandle;

	FInteractionData InteractionData;
	

	//==============================================================================
	// FUNCTIONS
	//==============================================================================
	
	virtual void BeginPlay();
	
	virtual void Tick(float DeltaSeconds) override;

	// INTERACTION -------------------------------------------------------------
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();
	//--------------------------------------------------------------------------

	/** Called for movement input */
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	
	
	
};

