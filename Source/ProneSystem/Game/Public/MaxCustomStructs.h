// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MaxCustomStructs.generated.h"

/**
 * 
 */
class PRONESYSTEM_API MaxCustomStructs
{
public:
	MaxCustomStructs();
	~MaxCustomStructs();
};

USTRUCT(Atomic, BlueprintType)
struct FMoveDirBlend
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Front;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Back;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Right;
};

USTRUCT(Atomic, BlueprintType)
struct FProneRotBlend
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Front;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Back;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Right;
};

