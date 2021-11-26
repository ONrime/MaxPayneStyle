// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Hand/PlayerHandStateBase.h"

UPlayerHandStateBase::UPlayerHandStateBase()
{
	PrimaryComponentTick.bCanEverTick = false;

}

UPlayerHandStateBase* UPlayerHandStateBase::HandleInput(APlayerCharacter* Player)
{
	return nullptr;
}

UPlayerHandStateBase* UPlayerHandStateBase::ChangeState(EPlayerHandState State)
{
	return nullptr;
}

void UPlayerHandStateBase::StateStart_Implementation(APlayerCharacter* Player)
{
}

void UPlayerHandStateBase::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UPlayerHandStateBase::StateEnd_Implementation(APlayerCharacter* Player)
{
}

UClass* UPlayerHandStateBase::GetStateClass_Implementation()
{
	return nullptr;
}

void UPlayerHandStateBase::PlayerFire(APlayerCharacter* Player)
{
}

void UPlayerHandStateBase::PlayerReload(APlayerCharacter* Player)
{
}
