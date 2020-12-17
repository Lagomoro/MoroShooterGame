// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "Components/CapsuleComponent.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GeneratedCodeHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Weapon/PickableWeapon.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom);

	CameraBoom->bUsePawnControlRotation = true;

	GetCapsuleComponent()->SetMassOverrideInKg(NAME_None, 75.0);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	GetMesh()->BodyInstance.bOverrideMass = true;
	GetMesh()->BodyInstance.SetMassOverride(0.f);

	PickupFunctionModule = CreateDefaultSubobject<UPickupFunctionModule>("PickupFunctionModule");
	PickupFunctionModule->OnPickupItemEvent.AddDynamic(this, &APlayerCharacter::HandlePickupOnServer);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, AimPitch);
	DOREPLIFETIME(APlayerCharacter, AimYaw);
	DOREPLIFETIME(APlayerCharacter, MaxHealth);
	DOREPLIFETIME(APlayerCharacter, Health);
	DOREPLIFETIME(APlayerCharacter, bDead);
	DOREPLIFETIME(APlayerCharacter, WeaponInHand);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();

	Health = MaxHealth;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled() || GetLocalRole() == ROLE_Authority)
	{
		FMinimalViewInfo DesiredView;
		FollowCamera->GetCameraView(0, DesiredView);
		const FRotator CameraRotation = DesiredView.Rotation - GetActorRotation();
		AimYaw = CameraRotation.Yaw;
		AimPitch = CameraRotation.Pitch;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlayerCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APlayerCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::OnFireEnd);

	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &APlayerCharacter::OnPickup);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &APlayerCharacter::OnDrop);

	// PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::);
	// PlayerInputComponent->BindAction("OpenInGameMenu", IE_Pressed, this, &APlayerCharacter::);
	// PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::);
	// PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::);
	// PlayerInputComponent->BindAction("FreeView", IE_Pressed, this, &APlayerCharacter::);
}

void APlayerCharacter::MoveForward(const float AxisValue)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), AxisValue * MovementInputFactor);
}

void APlayerCharacter::MoveRight(const float AxisValue)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), AxisValue * MovementInputFactor);
}

void APlayerCharacter::LookUpAtRate(const float AxisValue)
{
	AddControllerPitchInput(AxisValue * ViewInputFactor);
}

void APlayerCharacter::TurnAtRate(const float AxisValue)
{
	AddControllerYawInput(AxisValue * ViewInputFactor);
}

void APlayerCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APlayerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void APlayerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void APlayerCharacter::OnFireStart()
{
	HandleAction_Fire_StartOnServer();
}

void APlayerCharacter::OnFireEnd()
{
	HandleAction_Fire_StopOnServer();
}

void APlayerCharacter::OnPickup()
{
	HandleAction_PickupOnServer();
}

void APlayerCharacter::OnDrop()
{
	UE_LOG(LogTemp, Log, TEXT("Drop"));

	HandleDropOnServer();
}

bool APlayerCharacter::IsAttacking()
{
	if (IsValid(WeaponInHand))
	{
		return WeaponInHand->IsFiring();
	}
	return IsAttackingWithNoWeaponInHand_BP();
}

float APlayerCharacter::AddHealth(const float HealthToAdd)
{
	if (bDead)
	{
		return 0;
	}
	if (MaxHealth - Health > HealthToAdd)
	{
		Health += HealthToAdd;
		return HealthToAdd;
	}
	const float AddedHealth = MaxHealth - Health;
	Health = MaxHealth;
	return AddedHealth;
}

EWeaponType APlayerCharacter::GetWeaponType()
{
	return IsValid(WeaponInHand) ? WeaponInHand->WeaponType : EWeaponType::EMPTY;
}

void APlayerCharacter::OnAbleToPickupItem_Implementation(AActor* ItemAbleToPickup)
{
	PickupFunctionModule->AddItemToList(ItemAbleToPickup);
}

void APlayerCharacter::OnLostTrackOfItem_Implementation(AActor* ItemLostTrackOf)
{
	PickupFunctionModule->RemoveItemFromList(ItemLostTrackOf);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		if (HasAuthority())
		{
			const auto KillerPawn = IsValid(EventInstigator) ? EventInstigator->GetPawn() : nullptr;
			const auto KillerCharacter = IsValid(EventInstigator) ? EventInstigator->GetCharacter() : nullptr;
			ActionDie_BP_OnServer(KillerPawn, DamageCauser);
			ActionDie(KillerPawn, DamageCauser);
			if (IsValid(KillerCharacter) && KillerCharacter->GetClass()->IsChildOf(StaticClass()))
			{
				Cast<APlayerCharacter>(KillerCharacter)->OnConfirmedKill(this);
			}
		}
	}
	else
	{
		if (HasAuthority())
		{
			const auto KillerCharacter = IsValid(EventInstigator) ? EventInstigator->GetCharacter() : nullptr;
			if (IsValid(KillerCharacter) && KillerCharacter->GetClass()->IsChildOf(StaticClass()))
			{
				Cast<APlayerCharacter>(KillerCharacter)->OnConfirmedHit(this);
			}
		}
	}
	return DamageAmount;
}

void APlayerCharacter::OnConfirmedHit_Implementation(APawn* HitPawn)
{
	OnConfirmedHit_BP_OnClient(HitPawn);
}

void APlayerCharacter::OnConfirmedKill_Implementation(APawn* KilledPlayer)
{
	OnConfirmedKill_BP_OnClient(KilledPlayer);
}

void APlayerCharacter::ActionDie_Implementation(APawn* EventInstigator, AActor* DamageCauser)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	if (IsValid(WeaponInHand))
	{
		if (WeaponInHand->GetClass()->IsChildOf(APickableWeapon::StaticClass()))
		{
			HandleDropOnServer();
		}
	}
	bDead = true;
	AController* InstigatorController = GetController();
	if (IsValid(InstigatorController) && InstigatorController->GetClass()->IsChildOf(APlayerController::StaticClass()))
	{
		DisableInput(Cast<APlayerController>(InstigatorController));
	}
	ActionDie_BP_OnAll(EventInstigator, DamageCauser);
}

void APlayerCharacter::HandleAction_PickupOnServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleAction_PickupOnServer_Implementation"));
	PickupFunctionModule->PickupFirstItem();
}

void APlayerCharacter::HandleAction_Fire_StopOnServer_Implementation()
{
	if (IsValid(WeaponInHand))
	{
		WeaponInHand->ActionStopFire();
	}
}

void APlayerCharacter::HandleAction_Fire_StartOnServer_Implementation()
{
	if (IsValid(WeaponInHand))
	{
		WeaponInHand->ActionFire();
	}
}

void APlayerCharacter::CallItemInterfaceFunctionOnAll_Implementation(const bool Pickup, AActor* ItemForAction)
{
	IPickableItem* ActionItem = Cast<IPickableItem>(ItemForAction);
	if (Pickup)
	{
		ActionItem->Execute_OnPickup(ItemForAction, this);
	}
	else
	{
		ActionItem->Execute_OnDrop(ItemForAction);
	}
}

void APlayerCharacter::ChangeWeaponInHandOnAll_Implementation(const bool Attach)
{
	if (Attach)
	{
		WeaponInHand->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true),
			FName("GunSlot"));
	}
	else
	{
		WeaponInHand->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true));
	}
}

void APlayerCharacter::HandleDropOnServer_Implementation()
{
	if (IsValid(WeaponInHand))
	{
		ChangeWeaponInHandOnAll(false);
		CallItemInterfaceFunctionOnAll(false, WeaponInHand);
		WeaponInHand = nullptr;
	}
}

// Be called from PickupFunctionModule
void APlayerCharacter::HandlePickupOnServer_Implementation(AActor* ItemToPickup)
{
	UE_LOG(LogTemp, Warning, TEXT("Attach BroadCast"));
	if (ItemToPickup->GetClass()->IsChildOf(APickableWeapon::StaticClass()))
	{
		AWeaponBase* WeaponToPickup = Cast<AWeaponBase>(ItemToPickup);
		if (IsValid(WeaponInHand))
		{
			HandleDropOnServer();		
		}
		WeaponInHand = WeaponToPickup;
		ChangeWeaponInHandOnAll(true);
	}
	CallItemInterfaceFunctionOnAll(true, ItemToPickup);
	UE_LOG(LogTemp, Warning, TEXT("Attach BroadCast Over"));
}

