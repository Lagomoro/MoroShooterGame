// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponBase.h"

#include "GeneratedCodeHelpers.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WeaponBody = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponBody");
	WeaponFireArrow = CreateDefaultSubobject<UArrowComponent>("WeaponFireArrow");

	RootComponent = WeaponBody;
	WeaponFireArrow->SetupAttachment(RootComponent);

	WeaponBody->SetMassOverrideInKg(NAME_None, MassInKg);

	BulletRemain = MaxBulletCapacity;
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, bFiring);
	DOREPLIFETIME(AWeaponBase, bReloading);
	DOREPLIFETIME(AWeaponBase, BulletRemain);

	DOREPLIFETIME(AWeaponBase, WeaponType);
	DOREPLIFETIME(AWeaponBase, WeaponOwner);
	DOREPLIFETIME(AWeaponBase, MassInKg);

	DOREPLIFETIME(AWeaponBase, FireInterval);
	DOREPLIFETIME(AWeaponBase, bFireContinuously);
	DOREPLIFETIME(AWeaponBase, BulletCountPerFire);
	DOREPLIFETIME(AWeaponBase, BulletInterval);
	DOREPLIFETIME(AWeaponBase, bNeedsReload);
	DOREPLIFETIME(AWeaponBase, MaxBulletCapacity);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Fire Process
	UWorld* World = GetWorld();
	if (IsValid(World) && World->IsServer())
	{
		// Bullet In One Shot
		if (BulletCountSinceLastFire > 0 && BulletCountSinceLastFire < BulletCountPerFire)
		{
			if (TimeSinceLastBullet < BulletInterval * 2)
			{
				TimeSinceLastBullet += DeltaTime;
			}
			if (TimeSinceLastBullet > BulletInterval)
			{
				TimeSinceLastBullet = FMath::Fmod(TimeSinceLastBullet, BulletInterval);
				BulletCountSinceLastFire++;
				OnFire();
			}
		}
		else {
			// Fire
			if (TimeSinceLastFire < FireInterval * 2)
			{
				TimeSinceLastFire += DeltaTime;
			}
			if (bFiring && TimeSinceLastFire > FireInterval)
			{
				TimeSinceLastFire = FMath::Fmod(TimeSinceLastFire, FireInterval);
				TimeSinceLastBullet = TimeSinceLastFire;
				BulletCountSinceLastFire = 1;
				OnFire();
				if (!bFireContinuously)
				{
					TimeSinceLastFire = 0;
					ActionStopFire();
				}
			}
		}
	}
}

int AWeaponBase::UseBullet(const float ExpectedCount)
{
	if (!bNeedsReload)
	{
		return ExpectedCount;
	}
	if (BulletRemain < ExpectedCount)
	{
		const int Count = BulletRemain;
		BulletRemain = 0;
		return Count;
	}
	BulletRemain -= ExpectedCount;
	return ExpectedCount;
}

FRotator AWeaponBase::GetGunPortRotation()
{
	return WeaponFireArrow->GetComponentRotation();
}

FVector AWeaponBase::GetGunPortDirection()
{
	return WeaponFireArrow->GetComponentRotation().RotateVector(FVector::ForwardVector);
}

FVector AWeaponBase::GetGunPortLocation()
{
	return WeaponFireArrow->GetComponentLocation();
}

void AWeaponBase::OnReloadStart_Implementation()
{
	OnReloadStart_BP();
}

void AWeaponBase::OnReloadComplete_Implementation()
{
	OnReloadComplete_BP();
}

void AWeaponBase::OnReloadInterrupt_Implementation()
{
	OnReloadInterrupt_BP();
}

void AWeaponBase::OnFire_Implementation()
{
	OnFire_BP();
}

bool AWeaponBase::IsFiring_Implementation()
{
	return bFiring;
}

void AWeaponBase::ActionFire_Implementation()
{
	if (!bFiring)
	{
		if (bReloading)
		{
			if (BulletRemain > 0)
			{
				ActionStopReload(true);
			}
			else
			{
				return;
			}
		}
		bFiring = true;
	}
}

void AWeaponBase::ActionStopFire_Implementation()
{
	bFiring = false;
}

void AWeaponBase::ActionReload_Implementation()
{
	if (bNeedsReload && !bReloading)
	{
		if (BulletRemain < MaxBulletCapacity)
		{
			bReloading = true;
			if (bFiring) ActionStopFire();
			OnReloadStart();
		}
	}
}

void AWeaponBase::ActionStopReload_Implementation(const bool GenerateInterruption)
{
	if (bReloading)
	{
		bReloading = false;
		OnReloadInterrupt();
		if (GenerateInterruption)
		{
			OnReloadInterrupted.Broadcast();
		}
	}
}