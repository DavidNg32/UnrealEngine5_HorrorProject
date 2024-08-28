// Copyright Epic Games, Inc. All Rights Reserved.

// GAME ---------------------------------------------------------------------
#include "Player/GameCharacter.h"
#include "Interfaces/InteractionInterface.h"
#include "User Interface/Game_HUD.h"
//---------------------------------------------------------------------------

// ENGINE BASED -------------------------------------------------------------
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "DrawDebugHelpers.h"
//---------------------------------------------------------------------------

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGameCharacter

AGameCharacter::AGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//set fov
	FirstPersonCameraComponent->SetFieldOfView(90.0f);

	//INTERACTION DATA -----------------------------------------------------
	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 100.0f;
	//------------------------------------------------------------------------

}

//////////////////////////////////////////////////////////////////////////// Input

void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameCharacter::Look);

		//Interact
		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGameCharacter::BeginInteract);
		PlayerInputComponent->BindAction("Interact", IE_Released, this, &AGameCharacter::EndInteract);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is "
										   "built to use the Enhanced Input system. If you intend to use the legacy "
			 "system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		
		
	}
}

void AGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
		// add yaw and pitch input to controller
		
		
	}
}

void AGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	HUD = Cast<AGame_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
}

void AGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) >= InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

void AGameCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ FirstPersonCameraComponent->GetComponentLocation() };
	FVector TraceEnd{ TraceStart + FirstPersonCameraComponent->GetForwardVector() * InteractionCheckDistance };

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult TraceHit;

	if(GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if(TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			

			if(TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(TraceHit.GetActor());
				return;
			}

			if(TraceHit.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}

	NoInteractableFound();

}

void AGameCharacter::FoundInteractable(AActor* NewInteractable)
{
	if(IsInteracting())
	{
		EndInteract();
	}

	if(InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;
	HUD->UpdateInteractionWidgetText(&TargetInteractable->InteractableData);
	TargetInteractable->BeginFocus();
}

void AGameCharacter::NoInteractableFound()
{
	if(IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(InteractionCheckTimerHandle);
		
	}
	if(InteractionData.CurrentInteractable)
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AGameCharacter::BeginInteract()
{
	// Verify that the interactable is still valid
	PerformInteractionCheck();

	if(InteractionData.CurrentInteractable)
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if(FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(InteractionCheckTimerHandle, this, &AGameCharacter::Interact,
						TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void AGameCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(InteractionCheckTimerHandle);

	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AGameCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(InteractionCheckTimerHandle);

	if(IsValid(TargetInteractable.GetObject()))
	{
		
		TargetInteractable->Interact(this);
	}
}

