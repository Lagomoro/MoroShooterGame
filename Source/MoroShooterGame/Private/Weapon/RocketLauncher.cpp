// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/RocketLauncher.h"
#include "GameFramework/Character.h"
#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"

// This event is executed on server.
void ARocketLauncher::OnFire_Implementation()
{
	Super::OnFire_Implementation();
	const float ActualUse = UseBullet(1);
	MakeFireEffectOnAll(ActualUse > 0);
}