// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableSlot.generated.h"

UINTERFACE(Blueprintable)
class UPickableSlot : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IPickableSlot
{
	GENERATED_IINTERFACE_BODY()

	//================================================================================
	// * Status£º
	//     PickableSlot Status
	//--------------------------------------------------------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	    void OnAbleToPickupItem(AActor* PickableItem);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	    void OnLostTrackOfItem(AActor* PickableItem);
	//================================================================================

};