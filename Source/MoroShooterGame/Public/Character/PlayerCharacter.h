// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/WeaponType.h"
#include "PickupFunctionModule.h"
#include "GameFramework/Character.h"
#include "Pickable/PickableSlot.h"
#include "Weapon/WeaponBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MOROSHOOTERGAME_API APlayerCharacter : public ACharacter, public IPickableSlot
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	//================================================================================
	// * Components£º
	//     PlayerCharacter Components
	//-------------------------------------------------------------------------------- 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		USpringArmComponent* CameraBoom;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UCameraComponent* FollowCamera;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UPickupFunctionModule* PickupFunctionModule;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UCameraComponent* Camera;
	//================================================================================

	//================================================================================
	// * Settings£º
	//     PlayerCharacter Settings
	//-------------------------------------------------------------------------------- 
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Settings")
		float MaxHealth = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		float MovementInputFactor = 1.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		float ViewInputFactor = 1.0;
	//================================================================================

	//================================================================================
	// * Status£º
	//     PlayerCharacter Status
	//-------------------------------------------------------------------------------- 
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Status|View")
		float AimPitch;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Status|View")
		float AimYaw;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Status")
		float Health;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Status")
		bool bDead;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Status")
		AWeaponBase* WeaponInHand;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		bool JumpButtonDown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		bool bFreeView;
	//================================================================================

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void OnAbleToPickupItem_Implementation(AActor* ItemAbleToPickup) override;
	virtual void OnLostTrackOfItem_Implementation(AActor* ItemLostTrackOf) override;

	//================================================================================
	// * Inputs£º
	//     PlayerCharacter Inputs
	//-------------------------------------------------------------------------------- 
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void MoveForward(const float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void MoveRight(const float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void LookUpAtRate(const float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void TurnAtRate(const float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void OnResetVR();
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void OnFireStart();
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void OnFireEnd();
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void OnPickup();
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		virtual void OnDrop();
	//================================================================================

	//================================================================================
	// * Status£º
	//     PlayerCharacter Status
	//-------------------------------------------------------------------------------- 
	UFUNCTION(BlueprintCallable, Category = Status)
		bool IsAttacking();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Status)
		bool IsAttackingWithNoWeaponInHand_BP();
	UFUNCTION(BlueprintCallable, Category = Status)
		float GetHealthPercentage() const { return Health / MaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Status)
		float GetHealthFromPercentage(const float Percentage) const { return MaxHealth * Percentage; }
	UFUNCTION(BlueprintCallable, Category = Status)
		float AddHealth(const float HealthToAdd);
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = Status)
		EWeaponType GetWeaponType();
	//================================================================================

	//================================================================================
	// * Process£º
	//     PlayerCharacter Process
	//-------------------------------------------------------------------------------- 
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Process")
		void HandlePickupOnServer(AActor* ItemToPickup);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Process")
		void HandleDropOnServer();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Process")
		void ChangeWeaponInHandOnAll(const bool Attach);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Process")
		void CallItemInterfaceFunctionOnAll(const bool Pickup, AActor* ItemForAction);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Process")
		void HandleAction_Fire_StartOnServer();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Process")
		void HandleAction_Fire_StopOnServer();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Process")
		void HandleAction_PickupOnServer();
	//================================================================================

	//================================================================================
	// * Actions£º
	//     PlayerCharacter Actions
	//-------------------------------------------------------------------------------- 
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Actions")
		void TakeDamage_BP(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Actions")
		virtual void ActionDie(APawn* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, Category = "Actions")
		void ActionDie_BP_OnAll(APawn* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, Category = "Actions")
		void ActionDie_BP_OnServer(APawn* EventInstigator, AActor* DamageCauser);
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Actions")
		virtual void OnConfirmedKill(APawn* KilledPawn);
	UFUNCTION(BlueprintImplementableEvent, Category = "Actions")
		void OnConfirmedKill_BP_OnClient(APawn* KilledPawn);
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Actions")
		virtual void OnConfirmedHit(APawn* HitPawn);
	UFUNCTION(BlueprintImplementableEvent, Category = "Actions")
		void OnConfirmedHit_BP_OnClient(APawn* HitPawn);
	//================================================================================

};
