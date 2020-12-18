// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/VerticalBox.h"

#include "PickupFunctionModule.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickupItemDelegate, AActor*, ItemToPickup);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOROSHOOTERGAME_API UPickupFunctionModule : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickupFunctionModule();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	TMap<AActor*, UUserWidget*> PickableWidgets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	TArray<AActor*> PickableItems;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	bool bShouldScanPickable = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Status)
	UVerticalBox* DisplayBox;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Settings)
	TSubclassOf<UUserWidget> PickableDisplayLabelClass;

	bool bDisplayLabelClassProperlySetup = false;
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Settings)
	FOnPickupItemDelegate OnPickupItemEvent;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category=Actions)
	virtual bool AddItemToList(AActor* ItemAbleToPickup);

	UFUNCTION(BlueprintCallable, Category=Actions)
    virtual bool RemoveItemFromList(AActor* ItemLostTrack);

	UFUNCTION(BlueprintCallable, Category=Actions)
    virtual bool PickupItem(AActor* ItemToPickup);

	UFUNCTION(BlueprintCallable, Category=Actions)
    virtual bool PickupFirstItem();
		
};
