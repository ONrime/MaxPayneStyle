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

	// ���� ��ȯ
	virtual UPlayerLowerStateBase* HandleInput(class APlayerCharacter* Player) override; // ��ư �Է¿� ���� ���� ��ȯ
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press) override;         // ��ư �Է¿� ���� ���� ��ȯ(��Ƽ�÷��̿�)    
	virtual UPlayerLowerStateBase* ChangeState(EPlayerLowerState State) override;        // ���� ���� ��ȯ(���ǿ� ���� ��ȯ�� �� �� ����Ѵ�.)

	// ���¿� ���� ����
	virtual void StateStart_Implementation(class APlayerCharacter* Player) override;
	virtual void StateUpdate_Implementation(class APlayerCharacter* Player, float DeltaSecond) override;
	virtual void StateEnd_Implementation(class APlayerCharacter* Player) override;

	virtual UClass* GetStateClass_Implementation() override;

};
