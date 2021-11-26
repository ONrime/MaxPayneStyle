// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Public/PlayerStateBase.h"
#include "Player/Public/PlayerCharacter.h"
#include "PlayerHandStateBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRONESYSTEM_API UPlayerHandStateBase : public UActorComponent, public IPlayerStateBase
{
	GENERATED_BODY()

public:
	UPlayerHandStateBase();

	// ���� ��ȯ
	virtual UPlayerHandStateBase* HandleInput(class APlayerCharacter* Player); // ��ư �Է¿� ���� ���� ��ȯ
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press);         // ��ư �Է¿� ���� ���� ��ȯ(��Ƽ�÷��̿�)    
	virtual UPlayerHandStateBase* ChangeState(EPlayerHandState State);        // ���� ���� ��ȯ(���ǿ� ���� ��ȯ�� �� �� ����Ѵ�.)

	// ���¿� ���� ����
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

	virtual void PlayerFire(class APlayerCharacter* Player);
	virtual void PlayerReload(class APlayerCharacter* Player);

	EPlayerHandState GetEState() { return HandState; }
	//EPlayerHandState GetEBeforeState() { return HandBeforeState; }
	//void SetEBeforeState(EPlayerUpperState Set) { HandBeforeState = Set; }

protected:
	UPlayerHandStateBase* temp = nullptr;
	EPlayerHandState HandState;
	//EPlayerHandState HandBeforeState;
};
