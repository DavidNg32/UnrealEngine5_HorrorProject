// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "TestActor.generated.h"

UCLASS()
class GAME_API ATestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	// Called every frame
	

protected:

	UPROPERTY(EditAnywhere, Category= "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category= "Test Actor")
	FInteractableData InstanceInteractableData;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(AGameCharacter* PlayerCharacter) override;
	

	
};
