// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Public/State/Lower/PlayerLowerStateBase.h"
#include "Prone_LowerState.generated.h"

/**
 * 
 */
UCLASS()
class PRONESYSTEM_API UProne_LowerState : public UPlayerLowerStateBase
{
	GENERATED_BODY()

public:
	UProne_LowerState();

	// 상태 변환
	virtual UPlayerLowerStateBase* HandleInput(class APlayerCharacter* Player) override; // 버튼 입력에 따른 상태 변환
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press) override;         // 버튼 입력에 따른 상태 변환(멀티플레이용)    
	virtual UPlayerLowerStateBase* ChangeState(EPlayerLowerState State) override;        // 강제 상태 변환(조건에 따른 변환을 할 때 사용한다.)

	// 상태에 따른 반응
	virtual void StateStart_Implementation(class APlayerCharacter* Player) override;
	virtual void StateUpdate_Implementation(class APlayerCharacter* Player, float DeltaSecond) override;
	virtual void StateEnd_Implementation(class APlayerCharacter* Player) override;

	virtual UClass* GetStateClass_Implementation() override;

};
