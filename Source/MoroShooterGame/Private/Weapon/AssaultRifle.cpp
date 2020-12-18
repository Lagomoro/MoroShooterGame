// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AssaultRifle.h"
#include "GameFramework/Character.h"
#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

// This event is executed on server.
void AAssaultRifle::OnFire_Implementation()
{
	Super::OnFire_Implementation();
	const float ActualUse = UseBullet(1);
	MakeFireEffectOnAll(ActualUse > 0);
	UWorld* World = GetWorld();
	if (ActualUse > 0)
	{
		if (IsValid(World))
		{
			FHitResult HitResult;
			const FVector GunPortLocation = WeaponFireArrow->GetComponentLocation();
			const FVector AimingDirection = WeaponFireArrow->GetForwardVector();
			auto QueryParams = FCollisionQueryParams(FName(), true, this);
			QueryParams.bReturnPhysicalMaterial = true;
			if (World->LineTraceSingleByChannel(HitResult, GunPortLocation + AimingDirection * 6.0,
				GunPortLocation + AimingDirection * ShootingRange, ECC_Visibility, QueryParams))
			{
				UPrimitiveComponent* HitComponent = HitResult.Component.Get();
				AActor* HitActor = HitResult.Actor.Get();
				const FName HitBone = HitResult.BoneName;
				MakeHitEffectOnAll(HitResult.PhysMaterial.Get(), HitResult.Location,
					HitActor, HitComponent, HitBone);
				
				// Apply impulse on server
				if (HitComponent->IsSimulatingPhysics() && HitComponent->Mobility == EComponentMobility::Movable)
				{
					HitComponent->AddImpulseAtLocation(HitResult.Normal * BulletImpulse,
						HitResult.ImpactPoint, HitBone);
				}

				UE_LOG(LogTemp, Log, TEXT("BeforeHit"));
				// Apply Damage
				float DamageToApply;
				ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
				if (IsValid(HitCharacter))	// Damage to Character
				{
					UE_LOG(LogTemp, Log, TEXT("AfterHit"));

					if (HitBone == FName("neck_01") || HitBone == FName("head") ||
						HitCharacter->GetMesh()->BoneIsChildOf(HitBone, FName("neck_01")) ||
						HitCharacter->GetMesh()->BoneIsChildOf(HitBone, FName("head")))
					{
						UE_LOG(LogTemp, Log, TEXT("HitHead"));
						DamageToApply = CriticalDamagePerFire;
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("HitBody"));
						DamageToApply = DamagePerFire;
					}
				}
				else	// Normal Actor
				{
					DamageToApply = DamagePerFire;
				}
				UGameplayStatics::ApplyDamage(HitActor, DamageToApply,
					IsValid(WeaponOwner) ? WeaponOwner->GetInstigatorController() : nullptr,
					this, UDamageType::StaticClass());
			}
		}
	}
	else
	{
	ActionStopFire();
	}
}


/*
bool AWeaponBase::CalculateFireDirectionByLineTrace(const FVector CameraLocation, const FRotator CameraRotation, float TraceRange)
{
	const FVector CameraDirection = CameraRotation.RotateVector(FVector::ForwardVector).GetSafeNormal();
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		FHitResult HitResult;
		if (World->LineTraceSingleByChannel(HitResult, CameraLocation + CameraDirection,
			CameraLocation + CameraDirection * TraceRange, ECC_Visibility,
			FCollisionQueryParams(FName(), true, this)))
		{
			SetFireRotation((HitResult.Location - GetGunPortLocation()).Rotation());
			return true;
		}
	}
	return false;
}

void AWeaponBase::CalculateFireDirectionByAdjustmentAlgorithm(const FVector CameraLocation, const FRotator CameraRotation)
{
	SetFireRotation((CameraRotation.RotateVector(FVector::ForwardVector).GetSafeNormal() * 1000 +
		CameraLocation - GetGunPortLocation()).Rotation());
}
*/
