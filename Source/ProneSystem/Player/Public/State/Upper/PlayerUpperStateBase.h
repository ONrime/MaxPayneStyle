// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Public/PlayerStateBase.h"
#include "Player/Public/PlayerCharacter.h"
#include "PlayerUpperStateBase.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRONESYSTEM_API UPlayerUpperStateBase : public UActorComponent, public IPlayerStateBase
{
	GENERATED_BODY()

public:	
	UPlayerUpperStateBase();

	// 상태 변환
	virtual UPlayerUpperStateBase* HandleInput(class APlayerCharacter* Player); // 버튼 입력에 따른 상태 변환
	//virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press);         // 버튼 입력에 따른 상태 변환(멀티플레이용)    
	virtual UPlayerUpperStateBase* ChangeState(EPlayerUpperState State);        // 강제 상태 변환(조건에 따른 변환을 할 때 사용한다.)
	
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

	// AnimIns
	virtual void AnimInsUpdate(class APlayerCharacter* Player, float& RootYaw, float& AimYaw, float& UpperYaw);

	virtual void PlayerFire(class APlayerCharacter* Player);

	EPlayerUpperState GetEState() { return UpperState; }
	EPlayerUpperState GetEBeforeState() { return UpperBeforeState; }
	void SetEBeforeState(EPlayerUpperState Set) { UpperBeforeState = Set; }

protected:
	UPlayerUpperStateBase* temp = nullptr;
	EPlayerUpperState UpperState;
	EPlayerUpperState UpperBeforeState;
		
};
