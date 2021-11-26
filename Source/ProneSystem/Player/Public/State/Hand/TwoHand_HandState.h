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

	// ���� ��ȯ
	virtual UPlayerHandStateBase* HandleInput(class APlayerCharacter* Player) override; // ��ư �Է¿� ���� ���� ��ȯ
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press) override;         // ��ư �Է¿� ���� ���� ��ȯ(��Ƽ�÷��̿�)    
	virtual UPlayerHandStateBase* ChangeState(EPlayerHandState State) override;        // ���� ���� ��ȯ(���ǿ� ���� ��ȯ�� �� �� ����Ѵ�.)

	// ���¿� ���� ����
	virtual void StateStart_Implementation(class APlayerCharacter* Player) override;
	virtual void StateUpdate_Implementation(class APlayerCharacter* Player, float DeltaSecond) override;
	virtual void StateEnd_Implementation(class APlayerCharacter* Player) override;

	virtual void PlayerFire(class APlayerCharacter* Player) override;
	virtual void PlayerReload(class APlayerCharacter* Player) override;

	virtual UClass* GetStateClass_Implementation() override;

};
