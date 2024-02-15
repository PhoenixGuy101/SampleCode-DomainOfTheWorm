// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "Enums/ItemEnum.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DOMAINOFTHEWORM_API AItem : public APickup
{
	GENERATED_BODY()
	
public:
	AItem();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact(APawn* interactor) override; //Interaction function

	virtual bool CanInteract() override; //returns a bool based on variable CanCollect

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	EItemEnum ItemType; //the item type of this actor

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	bool CanCollect = true; //if the item can be collected

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 StackAmount = 1; //how many of the item is currently stored by this actor

	virtual void BeginPlay() override;
	virtual void NoItemsLeft();
};

