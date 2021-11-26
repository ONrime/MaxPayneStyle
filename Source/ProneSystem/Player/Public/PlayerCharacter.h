// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FPlayerStateCheck, int);

// �÷��̾� ��ü ���� ǥ�� �ϴ� ������
UENUM(BlueprintType)
enum class EPlayerUpperState : uint8 {
	UNARMED UMETA(DisplayName = "Unarmed"),
	ARMED UMETA(DisplayName = "Armed"),
	AIM UMETA(DisplayName = "Aim"),
	ADS UMETA(DisplayName = "ADS")
};

//�÷��̾� ��ü ���� ǥ�� �ϴ� ������
UENUM(BlueprintType)
enum class EPlayerLowerState : uint8 {
	STANDING UMETA(DisplayName = "Standing"),
	CROUCH UMETA(DisplayName = "Crouch"),
	SPLINT UMETA(DisplayName = "Splint"),
	SLIDING UMETA(DisplayName = "Sliding"),
	PRONE UMETA(DisplayName = "Prone")
};

// �÷��̾� �� ���� ǥ�� �ϴ� ������
UENUM(BlueprintType)
enum class EPlayerHandState : uint8 {
	ONEHAND UMETA(DisplayName = "OneHand"),
	TWOHAND UMETA(DisplayName = "TwoHand"),
	BOTHHAND UMETA(DisplayName = "BothHand")
};

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
	class UPlayerHandStateBase* HandState = nullptr;
	EPlayerUpperState UpperStateNowEnum;
	EPlayerHandState HandStateNowEnum;
	void UpperPress(class UPlayerUpperStateBase* State = nullptr);
	void LowerPress(class UPlayerLowerStateBase* State = nullptr);
	void HandPress(class UPlayerHandStateBase* State = nullptr);

	// �̵�
	void PlayerMove(bool IsAactive, float Forward, float Right, FVector& Dir, float Speed, float DeltaSecond);
	void MoveForward(float Value);
	void MoveRight(float Value);
	bool IsMove = true;
	float InputForwardDir = 0.0f;
	float InputRightDir = 0.0f;
	FVector MoveDir = FVector::ZeroVector;
	FVector BodyDir = FVector::ZeroVector;

	// ����
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// �Է�
	void PlayerProne();
	void PlayerCrouch();
	void PlayerOne1();
	void PlayerOne2();
	void PlayerTwo();
	void PlayerBoth();
	void PlayerADS();
	void PlayerUnADS();
	void PlayerFire();

// ������ �ܿ� �����ؾ� �Ǵ� ���� �� �Լ���
public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ����
	UClass* UpperStateNowClass = nullptr;
	UClass* LowerStateNowClass = nullptr;
	UClass* HandStateNowClass = nullptr;
	UClass* UpperStateBeforeClass = nullptr;
	UClass* LowerStateBeforeClass = nullptr;
	UClass* HandStateBeforeClass = nullptr;
	EPlayerUpperState GetUpperStateNowEnum() { return UpperStateNowEnum; }
	EPlayerHandState GetHandStateNowEnum() { return HandStateNowEnum; }
	class UPlayerUpperStateBase* GetUpperState() { return UpperState; }
	FPlayerStateCheck ChangeStateCheck; // ���� Ŭ���� ������ ���� ������ �� ���δ�.
	bool IsProne = false; // AnimIns�������

	// �̵�
	bool GetIsMove() { return IsMove; }
	FVector GetMoveDir() { return MoveDir; }
	FVector GetBodyDir() { return BodyDir; }

};
