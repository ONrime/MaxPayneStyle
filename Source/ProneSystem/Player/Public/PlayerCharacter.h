// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PRONESYSTEM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

// �����Ϳ� ����Ǵ� ������
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

// ���߾�� �ϴ� �⺻ ���� �� �Լ���
protected:
	virtual void BeginPlay() override;

	// ����
	class UPlayerUpperStateBase* UpperState = nullptr;
	class UPlayerLowerStateBase* LowerState = nullptr;
	void UpperPress(class UPlayerUpperStateBase* State = nullptr);
	void LowerPress(class UPlayerLowerStateBase* State = nullptr);

	// �̵�
	void PlayerMove(bool IsAactive, float Forward, float Right, FVector& Dir, float Speed, float DeltaSecond);
	void MoveForward(float Value);
	void MoveRight(float Value);
	bool IsMove = true;
	float InputForwardDir = 0.0f;
	float InputRightDir = 0.0f;
	FVector MoveDir = FVector::ZeroVector;

	// ����
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// Input
	void PlayerProne();

// ������ �ܿ� �����ؾ� �Ǵ� ���� �� �Լ���
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ����
	UClass* UpperStateNowClass = nullptr;
	UClass* LowerStateNowClass = nullptr;
	UClass* UpperStateBeforeClass = nullptr;
	UClass* LowerStateBeforeClass = nullptr;

	// �̵�
	bool GetIsMove() { return IsMove; }

};
