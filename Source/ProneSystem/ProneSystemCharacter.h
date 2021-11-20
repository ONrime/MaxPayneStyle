// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ProneSystemCharacter.generated.h"

UCLASS(config=Game)
class AProneSystemCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* CameraArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* MoveInputArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* BodyArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseState, meta = (AllowPrivateAccess = "true"))
	float PlayerSpeed = 70.0f;

public:
	AProneSystemCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY()
	class UProneUI_UserWidget* ProneUI_WB = nullptr;

	bool GetIsProne() { return IsProne; }
	FRotator GetCtrlRot() { return CtrlRot; }

	bool IsShowCollision = false;
	bool IsProneIK = true;
	bool IsMove = true;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	
	void PlayerProne();
	void PlayerCameraChange();
	void PlayerShowCollision();
	void PlayerDisableProneIK();
	void PlayerMove(bool IsAactive, float Forward, float Right, FVector& Dir, float Speed, float DeltaSecond);

	UPROPERTY(Replicated)
	bool IsProne = false;
	bool IsProneBack = false;
	bool IsTurn = false;
	bool IsCameraChange = false;
	//UPROPERTY(ReplicatedUsing = OnRep_CtrlRotChange)
	FRotator CtrlRot = FRotator::ZeroRotator;

	UPROPERTY()
	class UCharacter_AnimInstance* AnimIns = nullptr;

	TSubclassOf<class UProneUI_UserWidget> ProneUI_Class;

// 기본 변수들
protected:
	// Move 변수들
	float InputForwardDir = 0.0f;
	float InputRightDir = 0.0f;
	FVector MoveDir = FVector::ZeroVector;


public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendIsProne(bool Send);
	bool Server_SendIsProne_Validate(bool Send);
	void Server_SendIsProne_Implementation(bool Send);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendCtrlRot(FRotator Rot);
	bool NetMulticast_SendCtrlRot_Validate(FRotator Rot);
	void NetMulticast_SendCtrlRot_Implementation(FRotator Rot);

};

