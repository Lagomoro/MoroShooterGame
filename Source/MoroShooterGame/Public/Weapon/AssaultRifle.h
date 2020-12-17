// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ChannelWeapon.h"
#include "AssaultRifle.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MOROSHOOTERGAME_API AAssaultRifle : public AChannelWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	USoundCue* FireSoundEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings|Fire")
	USoundCue* MagEmptySoundEffect;
	
public:
	virtual void OnFire_Implementation() override;
};
