// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PRONESYSTEM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

// 에디터에 노출되는 변수들
public:
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* CameraArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* MoveInputArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* BodyArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerBase)
	float PlayerSpeed = 140.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.0f;

// 감추어야 하는 기본 변수 및 함수들
protected:
	virtual void BeginPlay() override;

	// 상태
	class UPlayerUpperStateBase* UpperState = nullptr;
	class UPlayerLowerStateBase* LowerState = nullptr;
	void UpperPress(class UPlayerUpperStateBase* State = nullptr);
	void LowerPress(class UPlayerLowerStateBase* State = nullptr);

	// 이동
	void PlayerMove(bool IsAactive, float Forward, float Right, FVector& Dir, float Speed, float DeltaSecond);
	void MoveForward(float Value);
	void MoveRight(float Value);
	bool IsMove = true;
	float InputForwardDir = 0.0f;
	float InputRightDir = 0.0f;
	FVector MoveDir = FVector::ZeroVector;

	// 시점
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// Input
	void PlayerProne();

// 에디터 외에 노출해야 되는 변수 및 함수들
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 상태
	UClass* UpperStateNowClass = nullptr;
	UClass* LowerStateNowClass = nullptr;
	UClass* UpperStateBeforeClass = nullptr;
	UClass* LowerStateBeforeClass = nullptr;

	// 이동
	bool GetIsMove() { return IsMove; }

};
