// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FPlayerStateCheck, int);

// 플레이어 상체 상태 표시 하는 열거형
UENUM(BlueprintType)
enum class EPlayerUpperState : uint8 {
	UNARMED UMETA(DisplayName = "Unarmed"),
	ARMED UMETA(DisplayName = "Armed"),
	AIM UMETA(DisplayName = "Aim"),
	ADS UMETA(DisplayName = "ADS")
};

//플레이어 하체 상태 표시 하는 열거형
UENUM(BlueprintType)
enum class EPlayerLowerState : uint8 {
	STANDING UMETA(DisplayName = "Standing"),
	CROUCH UMETA(DisplayName = "Crouch"),
	SPLINT UMETA(DisplayName = "Splint"),
	SLIDING UMETA(DisplayName = "Sliding"),
	PRONE UMETA(DisplayName = "Prone"),
	DODGE UMETA(DisplayName = "Dodge")
};

// 플레이어 손 상태 표시 하는 열거형
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
	class UPlayerHandStateBase* HandState = nullptr;
	EPlayerUpperState UpperStateNowEnum;
	EPlayerLowerState LowerStateNowEnum;
	EPlayerHandState HandStateNowEnum;
	void UpperPress(class UPlayerUpperStateBase* State = nullptr);
	void LowerPress(class UPlayerLowerStateBase* State = nullptr);
	void HandPress(class UPlayerHandStateBase* State = nullptr);

	// 이동
	void PlayerMove(bool IsAactive, float Forward, float Right, FVector& Dir, float Speed, float DeltaSecond);
	void MoveForward(float Value);
	void MoveRight(float Value);
	bool IsMove = true;
	float InputForwardDir = 0.0f;
	float InputRightDir = 0.0f;
	FVector MoveDir = FVector::ZeroVector;
	FVector BodyDir = FVector::ZeroVector;

	// 엎드리기
	FRotator ProneRot = FRotator::ZeroRotator;
	
	// 카메라
	FVector CurrentSpringArmLoc = FVector::ZeroVector;

	// 시점
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// 입력
	void PlayerProne();
	void PlayerCrouch();
	void PlayerDodge();
	void PlayerOne1();
	void PlayerOne2();
	void PlayerTwo();
	void PlayerBoth();
	void PlayerADS();
	void PlayerUnADS();
	void PlayerFire();

	// 에디터 외에 노출해야 되는 변수 및 함수들
public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 상태
	UClass* UpperStateNowClass = nullptr;
	UClass* LowerStateNowClass = nullptr;
	UClass* HandStateNowClass = nullptr;
	UClass* UpperStateBeforeClass = nullptr;
	UClass* LowerStateBeforeClass = nullptr;
	UClass* HandStateBeforeClass = nullptr;
	EPlayerUpperState GetUpperStateNowEnum() { return UpperStateNowEnum; }
	EPlayerLowerState GetLowerStateNowEnum() { return LowerStateNowEnum; }
	EPlayerHandState GetHandStateNowEnum() { return HandStateNowEnum; }
	class UPlayerUpperStateBase* GetUpperState() { return UpperState; }
	FPlayerStateCheck ChangeUpperStateCheck; // 상태 클래스 내에서 상태 변경할 때 쓰인다.
	FPlayerStateCheck ChangeLowerStateCheck; // 상태 클래스 내에서 상태 변경할 때 쓰인다.
	bool IsProne = false; // AnimIns에서사용
	bool IsDodge = false; // AnimIns에서사용

	// 이동
	bool GetIsMove() { return IsMove; }
	void SetIsMove(bool Set) { IsMove = Set; }
	FVector GetMoveDir() { return MoveDir; }
	FVector GetBodyDir() { return BodyDir; }

	// 엎드리기
	FRotator GetProneRot() { return ProneRot; }
	void SetProneRot(FRotator Set) { ProneRot = Set;}

	// 카메라
	FVector LowerSpringArmLoc = FVector::ZeroVector;
	FVector UpperSpringArmLoc = FVector::ZeroVector;
	float SpringArmLocSpeed = 0.0f;
	float PitchUpLimite = 80.0f;
	float PitchDownLimite = -70.0f;

};
