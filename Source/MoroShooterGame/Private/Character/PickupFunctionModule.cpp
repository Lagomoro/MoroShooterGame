// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PickupFunctionModule.h"

#include "GeneratedCodeHelpers.h"
#include "Components/VerticalBoxSlot.h"
#include "Character/PickableInfoWidget.h"
#include "Pickable/PickableItem.h"

// Sets default values for this component's properties
UPickupFunctionModule::UPickupFunctionModule()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPickupFunctionModule::BeginPlay()
{
	Super::BeginPlay();

	// Checks if the Display Label Class has implemented the standard interface.
	if (PickableDisplayLabelClass != nullptr)
	{
		auto ImplementedInterfaces = PickableDisplayLabelClass->Interfaces;
		for (int i = 0; i < ImplementedInterfaces.Num(); i++)
		{
			if (ImplementedInterfaces[i].Class->IsChildOf(UPickableInfoWidget::StaticClass()))
			{
				bDisplayLabelClassProperlySetup = true;
				break;
			}
		}
		if (!bDisplayLabelClassProperlySetup)
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected widget class for [PickableDisplayLabel] did not implement the "
				"standard interface (IPickableInfoWidget), information might be incomplete on widget!"));
		}
	}
}

// Called every frame
void UPickupFunctionModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Now, after the DisplayBox has been properly configured, the missed items can finally be placed.
	if (bShouldScanPickable && IsValid(DisplayBox) && IsValid(PickableDisplayLabelClass))
	{
		TArray<AActor*> OverlappingActors;
		GetOwner()->GetOverlappingActors(OverlappingActors, UPickableItem::StaticClass());
		for (int i = 0; i < OverlappingActors.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *OverlappingActors[i]->GetName());
			AddItemToList(OverlappingActors[i]);
		}
		bShouldScanPickable = false;
	}
}

bool UPickupFunctionModule::AddItemToList(AActor* ItemAbleToPickup)
{
	PickableItems.AddUnique(ItemAbleToPickup);
	UE_LOG(LogTemp, Warning, TEXT("PickUp"));
	if (PickableItems.AddUnique(ItemAbleToPickup) != INDEX_NONE)
	{
		AController* OwnerController = GetOwner()->GetInstigatorController();
		if (!IsValid(OwnerController) || !OwnerController->IsLocalPlayerController())
		{
			return true;
		}
	}
	return false;
}

bool UPickupFunctionModule::RemoveItemFromList(AActor* ItemLostTrack)
{
	PickableItems.Remove(ItemLostTrack);
	if (PickableItems.Remove(ItemLostTrack) > 0)
	{
		AController* OwnerController = GetOwner()->GetInstigatorController();
		if (!IsValid(OwnerController) || !OwnerController->IsLocalPlayerController())
		{
			return true;
		}
	}
	return false;
}

bool UPickupFunctionModule::PickupItem(AActor* ItemToPickup)
{
	if (PickableItems.Contains(ItemToPickup))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attach Pickup"));
		OnPickupItemEvent.Broadcast(ItemToPickup);
		return true;
	}
	return false;
}

bool UPickupFunctionModule::PickupFirstItem()
{
	UE_LOG(LogTemp, Warning, TEXT("PickupFirstItem"));
	if (PickableItems.Num() == 0) return false;
	UE_LOG(LogTemp, Warning, TEXT("Start Pickup"));
	return PickupItem(PickableItems[0]);
}

