// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EMPTY = 0                   UMETA(DisplayName = "Empty"),

    HAND = 1                    UMETA(DisplayName = "Hand"),
    ASSAULT_RIFLE = 2           UMETA(DisplayName = "Assualt Rifle"),
    SNIPER_RIFLE = 3            UMETA(DisplayName = "Sniper Rifle"),
    ROCKET_LAUNCHER = 4         UMETA(DisplayName = "Rocket Launcher"),
    PISTOL = 5					UMETA(DisplayName = "Pistol"),
};
