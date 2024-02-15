// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Inventoriable.h"
#include "UStructs/InventorySlot.h"
#include "InventoryComponent.generated.h"

class UItemStatsManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOMAINOFTHEWORM_API UInventoryComponent : public UActorComponent, public IInventoriable
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

#pragma region Property Functions

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int32 GetSize() { return Size; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int32 GetCurrentEquipSlot() { return CurrentEquipSlot; }

#pragma endregion

	//.................................................................
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//................................................................

#pragma region Inventoriable Interface Functions
	virtual int32 AddItem(EItemEnum itemType, int32 amount, int32 index, bool toEquip) override; //returns the overflow of items that couldn't be added as a negative number or 0

	virtual void RemoveItem(int32 index) override;

	virtual void GetItem(int32 index) override;
#pragma endregion

	//.................................................................

	virtual bool IsValidIndex(int32 index);

	//...............................................................

#pragma region Equip Functions

	virtual void ScrollEquip(int32 direction); //function for when player scrolls through the inventory

	virtual void SetEquip(int32 index);

#pragma endregion

	//..............................................................

protected:
#pragma region Fields
	UPROPERTY(EditAnywhere, Category = "Inventory Contents")
	int32 Size; //Size of the inventory; how many slots it has to carry items
	
	//UPROPERTY(EditDefaultsOnly, Instanced, Category = "Inventory Contents")
	TArray<struct FInventorySlot> InventoryArray; //Array that keeps track of inventory slots

	int32 CurrentEquipSlot = 0;
#pragma endregion

	//.................................................................

#pragma region Item Identification
	TObjectPtr<UItemStatsManager> ItemStatsManager; //ItemStatsManager is an actor component part of the gamemode, it contains all the information on all items


#pragma endregion

	//..................................................................
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InsertItem(int32 index);
		
private:
	void EmptyInventory();
};
