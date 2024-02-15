// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Item.h"
#include "Interfaces/Inventoriable.h"
#include "Components/InventoryComponent.h"

AItem::AItem()
{

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

//When nothing is left to collect
void AItem::NoItemsLeft()
{
	Destroy();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Interaction Function
void AItem::Interact(APawn* interactor)
{
	int32 returnedInt; //set up int to hold item overflow from InventoryInterface's AddItem() function

	if (GEngine) GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Orange, FString(interactor->GetName() + TEXT(" interacted with ") + this->GetName()));

	UInventoryComponent* InventoryComponent = interactor->GetComponentByClass<UInventoryComponent>(); //test for inventory component

	if (IInventoriable* InventoryInterface = Cast<IInventoriable>(InventoryComponent)) //test for Inventoriable Interface
	{
		returnedInt = InventoryInterface->AddItem(ItemType, 1, 0, true); //add 1 to the currently equipped slot of the inventory

		StackAmount -= (StackAmount + returnedInt); //stack amount updates based on the returned overflow.

		if (StackAmount <= 0)
		{
			NoItemsLeft();
		}

	}
	else {
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Orange, FString(TEXT("Item not collected")));
	} //*/
}

bool AItem::CanInteract()
{
	return CanCollect;
}
