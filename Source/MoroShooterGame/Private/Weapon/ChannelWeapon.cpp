// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ChannelWeapon.h"
#include "GameFramework/Character.h"
#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

void AChannelWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChannelWeapon, ShootingRange);
	DOREPLIFETIME(AChannelWeapon, BulletImpulse);
	DOREPLIFETIME(AChannelWeapon, DamagePerFire);
	DOREPLIFETIME(AChannelWeapon, CriticalDamagePerFire);
}

void AChannelWeapon::MakeHitEffectOnAll_Implementation(UPhysicalMaterial* HitPhysicalMaterial, FVector HitLocation,
	AActor* HitActor, UPrimitiveComponent* HitComponent, FName HitBoneName)
{
	MakeHitEffect_BP(HitPhysicalMaterial, HitLocation, HitActor, HitComponent, HitBoneName);
}

void AChannelWeapon::MakeFireEffectOnAll_Implementation(const bool Fired)
{
	MakeFireEffect_BP(Fired);
}
