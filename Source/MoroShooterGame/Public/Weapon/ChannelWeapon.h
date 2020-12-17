// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/PickableWeapon.h"
#include "ChannelWeapon.generated.h"

UCLASS()
class MOROSHOOTERGAME_API AChannelWeapon : public APickableWeapon
{
	GENERATED_BODY()

public:
	//================================================================================
	// * Fire Settings£º
	//     Weapon Fire Settings
	//--------------------------------------------------------------------------------
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		float ShootingRange;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		float BulletImpulse;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		float DamagePerFire;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		float CriticalDamagePerFire;
	//================================================================================

public:
	//================================================================================
	// * Process£º
	//     Weapon Process
	//--------------------------------------------------------------------------------
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Process")
		virtual void MakeFireEffectOnAll(const bool Fired);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Process")
		void MakeFireEffect_BP(const bool Fired);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Process")
		virtual void MakeHitEffectOnAll(UPhysicalMaterial* HitPhysicalMaterial, FVector HitLocation, AActor* HitActor,
			UPrimitiveComponent* HitComponent, FName HitBoneName);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Process")
		void MakeHitEffect_BP(UPhysicalMaterial* HitPhysicalMaterial, FVector HitLocation, AActor* HitActor,
			UPrimitiveComponent* HitComponent, FName HitBoneName);
	//================================================================================

};
