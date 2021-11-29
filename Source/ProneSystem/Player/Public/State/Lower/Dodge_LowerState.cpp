// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Lower/Dodge_LowerState.h"
#include "Player/Public/State/Lower/Prone_LowerState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UDodge_LowerState::UDodge_LowerState()
{
    LowerState = EPlayerLowerState::DODGE;
}

UPlayerLowerStateBase* UDodge_LowerState::HandleInput(APlayerCharacter* Player)
{
    return nullptr;
}

UPlayerLowerStateBase* UDodge_LowerState::ChangeState(EPlayerLowerState State)
{
	if (State == EPlayerLowerState::PRONE)
	{
		temp = NewObject<UProne_LowerState>(this, UProne_LowerState::StaticClass());
	}

	return temp;
}

void UDodge_LowerState::StateStart_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UDodge_LowerState: StateStart"));
	
	if (Player->GetProneRot() == FRotator::ZeroRotator) Player->SetProneRot(Player->GetMoveDir().Rotation());
	Player->IsDodge = true;
	PlayerCharacter = Player;
	Player->SetIsMove(false);

	GetWorld()->GetTimerManager().SetTimer(DodgeStartTimer, this, &UDodge_LowerState::StartDodge, 0.4f, false);

	
}

void UDodge_LowerState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
	if (IsDodgeStart && Player->GetMovementComponent()->IsFalling() == false)
	{
		PlayerCharacter->ChangeLowerStateCheck.Execute((int)EPlayerLowerState::PRONE);
	}
}

void UDodge_LowerState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UDodge_LowerState: StateEnd"));

	Player->SetIsMove(true);
	Player->IsDodge = false;
}

UClass* UDodge_LowerState::GetStateClass_Implementation()
{
    return UDodge_LowerState::StaticClass();
}

void UDodge_LowerState::StartDodge()
{
	FVector LaunchDir = FVector(PlayerCharacter->GetMoveDir().X * 600.0f, PlayerCharacter->GetMoveDir().Y * 600.0f, 350.0f);
	PlayerCharacter->LaunchCharacter(LaunchDir, false, false);
	IsDodgeStart = true;

}
