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

	// ���� ��ȯ
	virtual UPlayerUpperStateBase* HandleInput(class APlayerCharacter* Player) override; // ��ư �Է¿� ���� ���� ��ȯ
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press) override;         // ��ư �Է¿� ���� ���� ��ȯ(��Ƽ�÷��̿�)    
	virtual UPlayerUpperStateBase* ChangeState(EPlayerUpperState State) override;        // ���� ���� ��ȯ(���ǿ� ���� ��ȯ�� �� �� ����Ѵ�.)

	// ���¿� ���� ����
	virtual void StateStart_Implementation(class APlayerCharacter* Player) override;
	virtual void StateUpdate_Implementation(class APlayerCharacter* Player, float DeltaSecond) override;
	virtual void StateEnd_Implementation(class APlayerCharacter* Player) override;

	virtual UClass* GetStateClass_Implementation() override;
};
