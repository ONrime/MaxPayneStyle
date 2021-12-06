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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	bool IsMove = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	float TurnStart = 0.0f;
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
	bool IsDodge = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	bool IsProneBack = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	float PlayerSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Base, Meta = (AllowPrivateAccess = true))
	float InputSpeed = 0.0f;
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
	UClass* LowerStateBClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	UClass* UpperStateNClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	UClass* HandStateNClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FProneRotBlend ProneRot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	float ProneIK_Upper = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FRotator ProneIK_Upper_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FRotator ProneIK_Pelvis_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FRotator ProneIK_Right_Knee_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FRotator ProneIK_Left_Knee_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FRotator ProneIK_Right_Foot_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FRotator ProneIK_Left_Foot_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FRotator ProneIK_Right_Hand_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prone, Meta = (AllowPrivateAccess = true))
	FRotator ProneIK_Left_Hand_Rot = FRotator::ZeroRotator;


	// Move 처리
	void GetMoveDirBlend(FVector Velocity, FRotator Rot, FMoveDirBlend& Current);

	// 회전 처리
	void ArmedBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper);
	void AimBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper);
	void ADSBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper);
	void ProneBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper, FRotator DirEnd);
	void DodgeBodyYaw(class APlayerCharacter* Player, float& Root, float& Aim, float& Upper, FRotator DirEnd);

	bool IsTurn = false;
	FRotator TurnDir = FRotator::ZeroRotator;
	FRotator TurnDirEnd = FRotator::ZeroRotator;
	float RootRotInterpSpeed = 0.0f;

	// Prone 처리
	FProneRotBlend GetProneRotBlend(FVector PlayerForwardLoc, FRotator PlayerRot);
	 // Prone IK
	FVector ProneRootIK(class APlayerCharacter* Player, FName BoneName, FRotator& Rot, bool IsShow);
	FVector ProneMiddleIK(class APlayerCharacter* Player, FName BoneName, FRotator& Rot, FVector Dir, bool IsShow);
	void ProneEndIK(class APlayerCharacter* Player, FName BoneName, FRotator& Rot, FVector Dir, bool IsShow);
	 // 중심축과 지형지물에서 위를 가리키는 축 사이의 각을 구하는 함수입니다.
	float ProneDegrees(float ImpactNomalXY, float ImpactNomalZ, float DirXY, float DirZ, float XY);

};
