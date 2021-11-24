// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Lower/Standing_LowerState.h"
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
	//TArray <FInputActionKeyMapping> ActionADS = PlayerInput->GetKeysForAction(TEXT("ADS"));

	/*if (PlayerInput->InputKey(ActionADS[0].Key, EInputEvent::IE_Released, 1.0f, true)) {
		temp = NewObject<UAim_UpperState>(this, UAim_UpperState::StaticClass());
	}*/

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
