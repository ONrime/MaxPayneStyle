// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Public/PlayerStateBase.h"
#include "Player/Public/PlayerCharacter.h"
#include "PlayerLowerStateBase.generated.h"

//�÷��̾� ��ü ���� ǥ�� �ϴ� ������
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

	// ���� ��ȯ
	virtual UPlayerLowerStateBase* HandleInput(class APlayerCharacter* Player); // ��ư �Է¿� ���� ���� ��ȯ
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press);         // ��ư �Է¿� ���� ���� ��ȯ(��Ƽ�÷��̿�)    
	virtual UPlayerLowerStateBase* ChangeState(EPlayerLowerState State);        // ���� ���� ��ȯ(���ǿ� ���� ��ȯ�� �� �� ����Ѵ�.)

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

	EPlayerLowerState GetEState() { return LowerState; }
	EPlayerLowerState GetEBeforeState() { return LowerBeforeState; }
	void SetEBeforeState(EPlayerLowerState Set) { LowerBeforeState = Set; }

protected:
	UPlayerLowerStateBase* temp = nullptr;
	EPlayerLowerState LowerState;
	EPlayerLowerState LowerBeforeState;

};
