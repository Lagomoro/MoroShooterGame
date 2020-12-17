// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "Pickable/PickableItem.h"
#include "Sound/SoundCue.h"
#include "Weapon/WeaponBase.h"
#include "PickableWeapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MOROSHOOTERGAME_API APickableWeapon : public AWeaponBase, public IPickableItem
{
	GENERATED_BODY()

public:
	APickableWeapon();
	
	//================================================================================
	// * Components£º
	//     PickableWeapon Components
	//-------------------------------------------------------------------------------- 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		USphereComponent* PickupRange;
	//================================================================================

	//================================================================================
	// * Settings£º
	//     PickableWeapon Settings
	//-------------------------------------------------------------------------------- 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		USoundCue* PickupSoundEffect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		USoundCue* DropSoundEffect;
	//================================================================================

	//================================================================================
	// * Actions£º
	//     PickableWeapon Actions
	//-------------------------------------------------------------------------------- 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
		void OnPickupRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
		void OnPickupRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//================================================================================

	//================================================================================
	// * Process£º
	//     PickableWeapon Process
	//-------------------------------------------------------------------------------- 
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Process")
		void HandlePickupActionOnServer(AActor* ActionActor);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Process")
		void SetSimulatePhysicsOnAll(const bool SimulatePhysics);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Process")
		void AnnounceLostTrackOnAll();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Process")
		void HandleDropActionOnServer();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Process")
		void AnnounceAbleToPickupOnAll();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Process")
		void PlaySoundOnAll(const bool PickupSound);
	//================================================================================

	virtual void OnPickup_Implementation(AActor* ActionActor) override;
	virtual void OnDrop_Implementation() override;
	virtual FPickableItemDisplayData GetPickableInfo_Implementation() override;
	virtual bool IsOpenForPickup_Implementation() override;

};
