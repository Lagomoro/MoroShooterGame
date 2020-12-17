// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableItemDisplayData.h"
#include "PickableItem.generated.h"

UINTERFACE(Blueprintable)
class UPickableItem : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IPickableItem
{
	GENERATED_IINTERFACE_BODY()

	//================================================================================
	// * Actions£º
	//     PickableItem Actions
	//--------------------------------------------------------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	    void OnPickup(AActor* ActionActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
        void OnDrop();
	//================================================================================

	//================================================================================
	// * Status£º
	//     PickableItem Status
	//--------------------------------------------------------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	    FPickableItemDisplayData GetPickableInfo();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	    bool IsOpenForPickup();
	//================================================================================

};
