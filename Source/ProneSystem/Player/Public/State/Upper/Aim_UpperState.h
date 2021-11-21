// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Public/State/Upper/PlayerUpperStateBase.h"
#include "Aim_UpperState.generated.h"

/**
 * 
 */
UCLASS()
class PRONESYSTEM_API UAim_UpperState : public UPlayerUpperStateBase
{
	GENERATED_BODY()
	
public:
	UAim_UpperState();

	// 상태 변환
	virtual UPlayerUpperStateBase* HandleInput(class APlayerCharacter* Player) override; // 버튼 입력에 따른 상태 변환
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press) override;         // 버튼 입력에 따른 상태 변환(멀티플레이용)    
	virtual UPlayerUpperStateBase* ChangeState(EPlayerUpperState State) override;        // 강제 상태 변환(조건에 따른 변환을 할 때 사용한다.)

	// 상태에 따른 반응
	virtual void StateStart_Implementation(class APlayerCharacter* Player) override;
	virtual void StateUpdate_Implementation(class APlayerCharacter* Player, float DeltaSecond) override;
	virtual void StateEnd_Implementation(class APlayerCharacter* Player) override;

	virtual UClass* GetStateClass_Implementation() override;
};
