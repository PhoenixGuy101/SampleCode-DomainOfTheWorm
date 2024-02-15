// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/APlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Interfaces/Interactable.h"
#include "Components/InventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values and creates components
AAPlayerCharacter::AAPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Inventory Component Setup
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	//Camera Setup
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(GetCapsuleComponent());
	PlayerCamera->bUsePawnControlRotation = true;
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
}

// Called when the game starts or when spawned
void AAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	//Setup for the Controller subsystem
	if (PlayerController) {
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0); //Addition of the DefaultMappingContext to the Controller Subsystem
		}
	}
}

// Called every frame
void AAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawInteractTrace(); //Unique function to test if any interactables are visible
}


// Called to bind functionality to input
void AAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Setup for Action inputs to be binded to player functions; must be assigned in Blueprints
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AAPlayerCharacter::PlayerInteract);
		EnhancedInputComponent->BindAction(ScrollInventoryAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::ScrollThroughInventory);
	}
}

//Move Function
void AAPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>(); //FVector2D value won't change
	AddMovementInput(GetActorForwardVector(), DirectionValue.Y); // Forward/Backward
	AddMovementInput(GetActorRightVector(), DirectionValue.X); // Sideways
}

//Look Function
void AAPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>(); //FVector2D value won't change
	PlayerController->AddPitchInput(DirectionValue.Y); // Pitch
	PlayerController->AddYawInput(DirectionValue.X); // Yaw
}

//Interact Function
void AAPlayerCharacter::PlayerInteract(const FInputActionValue& Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Interacting")); //debug to test if input works

	if (CurrentInteractable != nullptr)
	{
		//we know that there's an interactable object in the crosshair
		CurrentInteractable->Interact(Cast<APawn>(this));
	}
}

void AAPlayerCharacter::DrawInteractTrace()
{
	//Set up Vectors and HitResult for Sphere Trace
	FVector Start;
	FRotator PlayerRot;
	PlayerController->GetPlayerViewPoint(Start, PlayerRot);
	FVector End = Start + (PlayerRot.Vector() * InteractDistance); 
	FHitResult Hit;

	if (GetWorld())
	{
		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 1.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, InteractIgnore, EDrawDebugTrace::ForOneFrame, Hit, true))
		{
			if (IInteractable* testForInterface = Cast<IInteractable>(Hit.GetActor()))
			{
				if (testForInterface->CanInteract()) //CanInteract() returns a bool
				{
					CurrentInteractable = testForInterface;
				}
				else CurrentInteractable = nullptr;
			}
			else CurrentInteractable = nullptr;
		}
		else CurrentInteractable = nullptr;
	}
	
}

//Inventory Scroll
void AAPlayerCharacter::ScrollThroughInventory(const FInputActionValue& Value)
{
	const float ScrollDirection = Value.Get<float>();
	
	//Inventory Component scrolls through inventory slots
	PlayerInventory->ScrollEquip(-ScrollDirection);
}
