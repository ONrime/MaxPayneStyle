// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Lower/Standing_LowerState.h"
#include "Player/Public/State/Lower/Crouch_LowerState.h"
#include "Player/Public/State/Lower/Prone_LowerState.h"
#include "Player/Public/State/Lower/Dodge_LowerState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UStanding_LowerState::UStanding_LowerState()
{
    LowerState = EPlayerLowerState::STANDING;
}

UPlayerLowerStateBase* UStanding_LowerState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionCrouch = PlayerInput->GetKeysForAction(TEXT("Crouch"));
	TArray <FInputActionKeyMapping> ActionProne = PlayerInput->GetKeysForAction(TEXT("Prone"));
	TArray <FInputActionKeyMapping> ActionDodge = PlayerInput->GetKeysForAction(TEXT("Dodge"));

	if (PlayerInput->IsPressed(ActionCrouch[0].Key)) {
		temp = NewObject<UCrouch_LowerState>(this, UCrouch_LowerState::StaticClass());
	}
	else if (PlayerInput->IsPressed(ActionProne[0].Key))
	{
		temp = NewObject<UProne_LowerState>(this, UProne_LowerState::StaticClass());
	}
	else if (PlayerInput->IsPressed(ActionDodge[0].Key))
	{
		temp = NewObject<UDodge_LowerState>(this, UDodge_LowerState::StaticClass());
	}


	return temp;
}

UPlayerLowerStateBase* UStanding_LowerState::ChangeState(EPlayerLowerState State)
{
    return nullptr;
}

void UStanding_LowerState::StateStart_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UStanding_LowerState: StateStart"));

	Player->PlayerSpeed = 100.f;

	// 카메라
	 // 상하 조절
	Player->LowerSpringArmLoc.Z = 80.0f;
	Player->SpringArmLocSpeed = 8.0f;
	 // 상하 시점 한계 지점 설정
	Player->PitchUpLimite = 80.0f;
	Player->PitchDownLimite = -45.0f;

}

void UStanding_LowerState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{

}

void UStanding_LowerState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UStanding_LowerState: StateEnd"));
}

UClass* UStanding_LowerState::GetStateClass_Implementation()
{
    return UStanding_LowerState::StaticClass();
}
