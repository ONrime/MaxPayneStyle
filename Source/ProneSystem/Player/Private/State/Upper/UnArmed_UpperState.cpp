// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Upper/UnArmed_UpperState.h"

UUnArmed_UpperState::UUnArmed_UpperState()
{
    UpperState = EPlayerUpperState::UNARMED;
}

UPlayerUpperStateBase* UUnArmed_UpperState::HandleInput(APlayerCharacter* Player)
{
    return nullptr;
}

UPlayerUpperStateBase* UUnArmed_UpperState::ChangeState(EPlayerUpperState State)
{
    return nullptr;
}

void UUnArmed_UpperState::StateStart_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UUnArmed_UpperState: StateStart"));
}

void UUnArmed_UpperState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UUnArmed_UpperState::StateEnd_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UUnArmed_UpperState: StateEnd"));
}

void UUnArmed_UpperState::AnimInsUpdate(APlayerCharacter* Player, float& RootYaw, float& AimYaw, float& UpperYaw)
{
}

UClass* UUnArmed_UpperState::GetStateClass_Implementation()
{
    return UUnArmed_UpperState::StaticClass();
}
