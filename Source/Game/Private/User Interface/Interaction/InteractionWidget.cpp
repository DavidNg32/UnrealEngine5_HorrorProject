// Fill out your copyright notice in the Description page of Project Settings.


#include "User Interface/Interaction/InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"


void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
	
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData)
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::IT_Use:
		break;
		
	case EInteractableType::IT_Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		if(InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
			
		}

		else
		{
			QuantityText->SetVisibility(ESlateVisibility::Visible);
			QuantityText->SetText(FText::Format(FText::FromString("x{0}"), InteractableData->Quantity));
			
		}
		
		break;
		
	case EInteractableType::IT_Talk:
		break;
		
	case EInteractableType::IT_Device:
		KeyPressText->SetText(FText::FromString("Hold"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Visible);
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		CurrentInteractionDuration = InteractableData->InteractionDuration;
		
		break;
		
	case EInteractableType::IT_Toggle:
		break;
		
	case EInteractableType::IT_None:
		break;
		
	default: ;
	}

	ActionText->SetText(InteractableData->InteractableAction);
	NameText->SetText(InteractableData->InteractableName);
	
}

float UInteractionWidget::UpdateInteractionProgress()
{
	if(CurrentInteractionDuration > 0.0f)
	{
		CurrentInteractionDuration -= GetWorld()->GetDeltaSeconds();
		return CurrentInteractionDuration;
	}

	return 0.0f;
	
}


