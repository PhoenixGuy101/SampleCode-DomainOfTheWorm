// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InventoryComponent.h"
#include "Components/ItemStatsManager.h"
#include "Kismet/GameplayStatics.h"
#include "Gamemodes/GameplayGamemode.h"

UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryArray.SetNum(Size); //set the InventoryArray size properly

	// /*
	if (AGameplayGamemode* gamemode = Cast<AGameplayGamemode>(UGameplayStatics::GetGameMode(GetWorld()))) {
		ItemStatsManager = gamemode->GetComponentByClass<UItemStatsManager>(); //the item's stats are stored by the ItemStatsManager Actor component, part of the Gamemode
	} //*/
	
	//EmptyInventory();
}

//empties the inventory
void UInventoryComponent::EmptyInventory()
{
	for (int i = 0; i < Size; i++) {
		InventoryArray[i].ItemInSlot = EItemEnum::None;
		InventoryArray[i].CurrentStackSize = 0;
	}
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

int32 UInventoryComponent::AddItem(EItemEnum itemType, int32 amount, int32 index, bool toEquip = false)
{
	int32 itemIndex;

	if (toEquip) itemIndex = CurrentEquipSlot; //the added item will be going to the equip slot
	else if (IsValidIndex(index))
	{
		itemIndex = index; //the added item will be going to the given index
	}
	else return -amount; //the return value denotes to the item that none of the amount given was collected.

	int32 stackMax = ItemStatsManager->GetStackMax(itemType); //how many of the item can go in one slot
	int32 itemOverflow; //how much overflow (returned as 0 or a negative number) if the stack is too big
	if (itemType == InventoryArray[itemIndex].ItemInSlot) itemOverflow = stackMax - (InventoryArray[itemIndex].CurrentStackSize + amount); 
	else
	{
		//TODO: Drop/swap the item that currently occupies the slot


		InventoryArray[itemIndex].ItemInSlot = itemType; //added itemtype is now the current one.
		InventoryArray[itemIndex].CurrentStackSize = 0;
		itemOverflow = stackMax - amount;
	}
	if (itemOverflow >= 0)
	{
		InventoryArray[itemIndex].CurrentStackSize += amount;
		return 0; //with no overflow, 0 is returned as all of the amount has been collected
	}
	else return itemOverflow; //itemOverflow is returned; what isn't collected is returned as a negative value (3 rocks not collected? return -3)
}

void UInventoryComponent::RemoveItem(int32 index)
{

}

void UInventoryComponent::GetItem(int32 index)
{
	
}

//tests to see validity of given index for the size of InventoryArray
bool UInventoryComponent::IsValidIndex(int32 index)
{
	if (index >= 0 && index < Size) return true;
	else return false;
}

void UInventoryComponent::InsertItem(int32 index)
{
	
}

//Given the scroll direction, change the currently equipped slot
void UInventoryComponent::ScrollEquip(int32 direction)
{
	if (direction > 0)
	{
		CurrentEquipSlot += 1;
		if (CurrentEquipSlot >= Size)
		{
			CurrentEquipSlot = 0;
		}
	}
	else if (direction < 0)
	{
		CurrentEquipSlot -= 1;
		if (CurrentEquipSlot < 0)
		{
			CurrentEquipSlot = Size - 1;
		}
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString(TEXT("Equipped Slot: ") + FString::FromInt(CurrentEquipSlot) + TEXT("\nItem Type: ") + ItemStatsManager->GetDisplayName(InventoryArray[CurrentEquipSlot].ItemInSlot).ToString() + TEXT("\nStack Size: ") + FString::FromInt(InventoryArray[CurrentEquipSlot].CurrentStackSize)));
}

void UInventoryComponent::SetEquip(int32 index)
{
	if (IsValidIndex(index)) 
	{
		CurrentEquipSlot = index;
	}
}

