// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Lower/Crouch_LowerState.h"
#include "Player/Public/State/Lower/Standing_LowerState.h"
#include "Player/Public/State/Lower/Prone_LowerState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UCrouch_LowerState::UCrouch_LowerState()
{
    LowerState = EPlayerLowerState::CROUCH;
}

UPlayerLowerStateBase* UCrouch_LowerState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionCrouch = PlayerInput->GetKeysForAction(TEXT("Crouch"));
	TArray <FInputActionKeyMapping> ActionProne = PlayerInput->GetKeysForAction(TEXT("Prone"));

	if (PlayerInput->IsPressed(ActionProne[0].Key))
	{
		temp = NewObject<UProne_LowerState>(this, UProne_LowerState::StaticClass());
	}
	else if (PlayerInput->InputKey(ActionCrouch[0].Key, EInputEvent::IE_Released, 1.0f, true)) {
		temp = NewObject<UStanding_LowerState>(this, UStanding_LowerState::StaticClass());
	}

	return temp;
}

UPlayerLowerStateBase* UCrouch_LowerState::ChangeState(EPlayerLowerState State)
{
    return nullptr;
}

void UCrouch_LowerState::StateStart_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UCrouch_LowerState: StateStart"));

    Player->PlayerSpeed = 30.0f;

	// 카메라
	 // 상하 조절
	Player->LowerSpringArmLoc.Z = 50.0f;
	Player->SpringArmLocSpeed = 8.0f;
	 // 상하 시점 한계 지점 설정
	Player->PitchUpLimite = 80.0f;
	Player->PitchDownLimite = -45.0f;

}

void UCrouch_LowerState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{

}

void UCrouch_LowerState::StateEnd_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UCrouch_LowerState: StateEnd"));

}

UClass* UCrouch_LowerState::GetStateClass_Implementation()
{
    return UCrouch_LowerState::StaticClass();
}
