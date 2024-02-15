// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "APlayerCharacter.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;
class UBoxComponent;
class UCameraComponent;
class IInteractable;

UCLASS()
class DOMAINOFTHEWORM_API AAPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAPlayerCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
#pragma region Inputs

	TObjectPtr<APlayerController> PlayerController;
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem;
	
#pragma region Blueprint Editable
	//All member variables here are mapping contexts or actions, which require the designer to set up in blueprints

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> ScrollInventoryAction;
#pragma endregion

#pragma endregion Variables utilized for player inputs

	//..................................................................

#pragma region Player Movement

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

#pragma endregion Functions used for movement

	//.................................................................

#pragma region Interaction Functions
	void PlayerInteract(const FInputActionValue& Value); //Function for the player to interact with an object on screen

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractDistance = 250.0f; //how far the player can interact with game objects

	void DrawInteractTrace(); //function to draw a sphere trace and changes the value of CurrentInteractable.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TArray<AActor*> InteractIgnore; //Things to ignore with the sphere trace from DrawInteractTrace()

	TObjectPtr<IInteractable> CurrentInteractable; //Holds the reference to and Determines if there's anything the player can interact with

#pragma endregion

	//..................................................................

	void ScrollThroughInventory(const FInputActionValue& Value); //Function for the player to scroll through their inventory slots

	//.................................................................


#pragma region Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryComponent> PlayerInventory; //Player's Inventory Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCamera;

#pragma endregion

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};