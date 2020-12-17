// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponType.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadInterruptSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMagEmptySignature, bool, bFiredLastTime);
UCLASS(Blueprintable, BlueprintType)
class MOROSHOOTERGAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

	//================================================================================
	// * Components£º
	//     Weapon Components
	//-------------------------------------------------------------------------------- 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UArrowComponent* WeaponFireArrow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		USkeletalMeshComponent* WeaponBody;
	//================================================================================

public:
	//================================================================================
	// * Status£º
	//     Weapon Status
	//-------------------------------------------------------------------------------- 
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Status")
		bool bFiring = false;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Status")
		bool bReloading = false;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Status")
		int BulletRemain = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float TimeSinceLastFire = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float TimeSinceLastBullet = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		int BulletCountSinceLastFire = 0;
	//================================================================================

	//================================================================================
	// * Settings£º
	//     Weapon Settings
	//-------------------------------------------------------------------------------- 
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings")
		EWeaponType WeaponType;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings")
		AActor* WeaponOwner;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings")
		float MassInKg;
	//================================================================================

	//================================================================================
	// * Fire Settings£º
	//     Weapon Fire Settings
	//--------------------------------------------------------------------------------
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		float FireInterval = 0.5;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		bool bFireContinuously;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		int BulletCountPerFire = 1;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		float BulletInterval = 0.1;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		bool bNeedsReload;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		float MaxBulletCapacity = 30;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Fire")
		FVector CameraShakeOnFire;
	//--------------------------------------------------------------------------------

protected:
	//================================================================================
	// * ActionBindings£º
	//     Weapon ActionBindings
    //   # MULTICASTED
	//--------------------------------------------------------------------------------
	UPROPERTY(BlueprintAssignable, Category = "ActionBindings")
		FReloadInterruptSignature OnReloadInterrupted;
	UPROPERTY(BlueprintAssignable, Category = "ActionBindings")
		FMagEmptySignature OnMagEmpty;
	//================================================================================

public:
	virtual void Tick(float DeltaTime) override;

	//================================================================================
	// * Functions£º
	//     Weapon Functions
	//--------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Functions|Bullet")
		virtual int UseBullet(const float ExpectedCount);
	UFUNCTION(BlueprintCallable, Category = "Functions|Fire")
		virtual FVector GetGunPortLocation();
	UFUNCTION(BlueprintCallable, Category = "Functions|Fire")
		virtual FRotator GetGunPortRotation();
	UFUNCTION(BlueprintCallable, Category = "Functions|Fire")
		virtual FVector GetGunPortDirection();
	//================================================================================

    //================================================================================
	// * Events£º
	//     Weapon Events
	//--------------------------------------------------------------------------------
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Events|Reload")
		virtual void OnReloadStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events|Reload")
		void OnReloadStart_BP();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Events|Reload")
		virtual void OnReloadComplete();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events|Reload")
		void OnReloadComplete_BP();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Events|Reload")
		virtual void OnReloadInterrupt();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events|Reload")
		void OnReloadInterrupt_BP();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Events|Fire")
		virtual void OnFire();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events|Fire")
		void OnFire_BP();
	//================================================================================

	//================================================================================
	// * Status£º
	//     Weapon Actions
	//--------------------------------------------------------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status|Fire")
		bool IsFiring();
	//================================================================================

	//================================================================================
	// * Actions£º
	//     Weapon Actions
	//--------------------------------------------------------------------------------
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Actions|Fire")
		virtual void ActionFire();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Actions|Fire")
		virtual void ActionStopFire();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Actions|Reload")
		virtual void ActionReload();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Actions|Reload")
		virtual void ActionStopReload(const bool GenerateInterruption);
	//================================================================================

};
