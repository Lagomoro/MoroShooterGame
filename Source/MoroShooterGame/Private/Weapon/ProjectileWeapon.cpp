// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ProjectileWeapon.h"
#include "GameFramework/Character.h"
#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

void AProjectileWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileWeapon, ShootingRange);
	DOREPLIFETIME(AProjectileWeapon, BulletImpulse);
	DOREPLIFETIME(AProjectileWeapon, DamagePerFire);
	DOREPLIFETIME(AProjectileWeapon, CriticalDamagePerFire);
}

void AProjectileWeapon::MakeHitEffectOnAll_Implementation(UPhysicalMaterial* HitPhysicalMaterial, FVector HitLocation,
	AActor* HitActor, UPrimitiveComponent* HitComponent, FName HitBoneName)
{
	MakeHitEffect_BP(HitPhysicalMaterial, HitLocation, HitActor, HitComponent, HitBoneName);
}

void AProjectileWeapon::MakeFireEffectOnAll_Implementation(const bool Fired)
{
	MakeFireEffect_BP(Fired);
}
