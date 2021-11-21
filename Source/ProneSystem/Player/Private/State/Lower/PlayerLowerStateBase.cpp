// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Lower/PlayerLowerStateBase.h"

UPlayerLowerStateBase::UPlayerLowerStateBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UPlayerLowerStateBase* UPlayerLowerStateBase::HandleInput(APlayerCharacter* Player)
{
	return nullptr;
}

UPlayerLowerStateBase* UPlayerLowerStateBase::ChangeState(EPlayerLowerState State)
{
	return nullptr;
}

void UPlayerLowerStateBase::StateStart_Implementation(APlayerCharacter* Player)
{
}

void UPlayerLowerStateBase::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UPlayerLowerStateBase::StateEnd_Implementation(APlayerCharacter* Player)
{
}

UClass* UPlayerLowerStateBase::GetStateClass_Implementation()
{
	return nullptr;
}
