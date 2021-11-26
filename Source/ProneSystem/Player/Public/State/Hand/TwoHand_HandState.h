// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Public/State/Hand/PlayerHandStateBase.h"
#include "TwoHand_HandState.generated.h"

/**
 * 
 */
UCLASS()
class PRONESYSTEM_API UTwoHand_HandState : public UPlayerHandStateBase
{
	GENERATED_BODY()

public:
	UTwoHand_HandState();

	// 상태 변환
	virtual UPlayerHandStateBase* HandleInput(class APlayerCharacter* Player) override; // 버튼 입력에 따른 상태 변환
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press) override;         // 버튼 입력에 따른 상태 변환(멀티플레이용)    
	virtual UPlayerHandStateBase* ChangeState(EPlayerHandState State) override;        // 강제 상태 변환(조건에 따른 변환을 할 때 사용한다.)

	// 상태에 따른 반응
	virtual void StateStart_Implementation(class APlayerCharacter* Player) override;
	virtual void StateUpdate_Implementation(class APlayerCharacter* Player, float DeltaSecond) override;
	virtual void StateEnd_Implementation(class APlayerCharacter* Player) override;

	virtual void PlayerFire(class APlayerCharacter* Player) override;
	virtual void PlayerReload(class APlayerCharacter* Player) override;

	virtual UClass* GetStateClass_Implementation() override;

};
