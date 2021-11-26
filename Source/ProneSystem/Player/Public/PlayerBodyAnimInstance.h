// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Game/Public/MaxCustomStructs.h"
#include "PlayerBodyAnimInstance.generated.h"

/**
 * 
 */

// 플레이어 입력 방향 표시 하는 열거형
UENUM(BlueprintType)
enum class EInpuDir : uint8 {
	FRONT UMETA(DisplayName = "Front"),
	BACK UMETA(DisplayName = "Back"),
	LEFT UMETA(DisplayName = "Left"),
	RIGHT UMETA(DisplayName = "Right")
};

UCLASS()
class PRONESYSTEM_API UPlayerBodyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerBodyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	bool IsFalling = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	bool IsHandUp = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	bool IsJumped = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float UpperYaw = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float UpperPitch = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float AimYaw = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float UpperProneYaw = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	bool IsProne = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	bool IsProneBack = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	float PlayerSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move, Meta = (AllowPrivateAccess = true))
	FMoveDirBlend MoveDirBlend;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	EInpuDir InputDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	float DirForward = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	float RootYaw = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	float DirRight = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	UClass* LowerStateNClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	UClass* UpperStateNClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	UClass* HandStateNClass = nullptr;

	// Move 처리
	void GetMoveDirBlend(FVector Velocity, FRotator Rot, FMoveDirBlend& Current);

	// 회전 처리
	void ArmedBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper);
	void AimBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper);
	void ADSBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper);
	void ProneBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper);

	bool IsTurn = false;
	FRotator TurnDir = FRotator::ZeroRotator;
	FRotator TurnDirEnd = FRotator::ZeroRotator;
	float RootRotInterpSpeed = 0.0f;

};
