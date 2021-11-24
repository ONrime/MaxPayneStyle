// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Upper/PlayerUpperStateBase.h"

UPlayerUpperStateBase::UPlayerUpperStateBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UPlayerUpperStateBase* UPlayerUpperStateBase::HandleInput(APlayerCharacter* Player)
{
	return nullptr;
}

UPlayerUpperStateBase* UPlayerUpperStateBase::ChangeState(EPlayerUpperState State)
{
	return nullptr;
}

void UPlayerUpperStateBase::StateStart_Implementation(APlayerCharacter* Player)
{
}

void UPlayerUpperStateBase::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UPlayerUpperStateBase::StateEnd_Implementation(APlayerCharacter* Player)
{
}

UClass* UPlayerUpperStateBase::GetStateClass_Implementation()
{
	return nullptr;
}

void UPlayerUpperStateBase::AnimInsUpdate(APlayerCharacter* Player, float& RootYaw, float& AimYaw, float& UpperYaw)
{
}

void UPlayerUpperStateBase::PlayerFire(APlayerCharacter* Player)
{
}
