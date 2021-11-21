// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Public/PlayerStateBase.h"
#include "Player/Public/PlayerCharacter.h"
#include "PlayerLowerStateBase.generated.h"

//플레이어 하체 상태 표시 하는 열거형
UENUM(BlueprintType)
enum class EPlayerLowerState : uint8 {
	STANDING UMETA(DisplayName = "Standing"),
	CROUCH UMETA(DisplayName = "Crouch"),
	SPLINT UMETA(DisplayName = "Splint"),
	SLIDING UMETA(DisplayName = "Sliding"),
	PRONE UMETA(DisplayName = "Prone")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRONESYSTEM_API UPlayerLowerStateBase : public UActorComponent, public IPlayerStateBase
{
	GENERATED_BODY()

public:	
	UPlayerLowerStateBase();

	// 상태 변환
	virtual UPlayerLowerStateBase* HandleInput(class APlayerCharacter* Player); // 버튼 입력에 따른 상태 변환
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press);         // 버튼 입력에 따른 상태 변환(멀티플레이용)    
	virtual UPlayerLowerStateBase* ChangeState(EPlayerLowerState State);        // 강제 상태 변환(조건에 따른 변환을 할 때 사용한다.)

	// 상태에 따른 반응
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State")
		void StateStart(class APlayerCharacter* Player);
	virtual void StateStart_Implementation(class APlayerCharacter* Player) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State")
		void StateUpdate(class APlayerCharacter* Player, float DeltaSecond);
	virtual void StateUpdate_Implementation(class APlayerCharacter* Player, float DeltaSecond) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State")
		void StateEnd(class APlayerCharacter* Player);
	virtual void StateEnd_Implementation(class APlayerCharacter* Player) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State")
		UClass* GetStateClass();
	virtual UClass* GetStateClass_Implementation() override;

	EPlayerLowerState GetEState() { return LowerState; }
	EPlayerLowerState GetEBeforeState() { return LowerBeforeState; }
	void SetEBeforeState(EPlayerLowerState Set) { LowerBeforeState = Set; }

protected:
	UPlayerLowerStateBase* temp = nullptr;
	EPlayerLowerState LowerState;
	EPlayerLowerState LowerBeforeState;

};
