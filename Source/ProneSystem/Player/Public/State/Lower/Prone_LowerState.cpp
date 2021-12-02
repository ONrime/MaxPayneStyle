// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Lower/Prone_LowerState.h"
#include "Player/Public/State/Lower/Crouch_LowerState.h"
#include "Player/Public/State/Lower/Standing_LowerState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UProne_LowerState::UProne_LowerState()
{
    LowerState = EPlayerLowerState::PRONE;
}

UPlayerLowerStateBase* UProne_LowerState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionCrouch = PlayerInput->GetKeysForAction(TEXT("Crouch"));
	TArray <FInputActionKeyMapping> ActionProne = PlayerInput->GetKeysForAction(TEXT("Prone"));

	if (PlayerInput->IsPressed(ActionCrouch[0].Key))
	{
		temp = NewObject<UCrouch_LowerState>(this, UCrouch_LowerState::StaticClass());
	}
	else if (PlayerInput->InputKey(ActionProne[0].Key, EInputEvent::IE_Released, 1.0f, true)) 
	{
		temp = NewObject<UStanding_LowerState>(this, UStanding_LowerState::StaticClass());
	}


	return temp;
}

UPlayerLowerStateBase* UProne_LowerState::ChangeState(EPlayerLowerState State)
{
    return nullptr;
}

void UProne_LowerState::StateStart_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UProne_LowerState: StateStart"));

	if (LowerBeforeState != EPlayerLowerState::DODGE) {
		Player->SetProneRot(Player->GetActorRotation());
		UE_LOG(LogTemp, Warning, TEXT("UProne_LowerState: DodgeCheck"));
	}
	Player->PlayerSpeed = 50.0f;
	Player->IsProne = true;
	if(Player->IsDodge) Player->IsDodge = false;

	// 카메라
	// 상하 조절
	Player->LowerSpringArmLoc.Z = 0.0f;
	Player->SpringArmLocSpeed = 8.0f;
}

void UProne_LowerState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
	if (Player->GetMoveDir().Size() >= 2.0f)
	{
		Player->SetProneRot(Player->GetActorRotation());
		//UE_LOG(LogTemp, Warning, TEXT("UProne_LowerState: DodgeCheck"));
	}

}

void UProne_LowerState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UProne_LowerState: StateEnd"));

	Player->IsProne = false;
	//Player->SetProneRot(FRotator::ZeroRotator);
}

UClass* UProne_LowerState::GetStateClass_Implementation()
{
    return UProne_LowerState::StaticClass();
}
