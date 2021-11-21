// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Upper/Armed_UpperState.h"
#include "Player/Public/State/Upper/ADS_UpperState.h"
#include "Player/Public/State/Upper/Aim_UpperState.h"
#include "Player/Public/State/Upper/UnArmed_UpperState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UArmed_UpperState::UArmed_UpperState()
{
	UpperState = EPlayerUpperState::ARMED;
}

UPlayerUpperStateBase* UArmed_UpperState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionADS = PlayerInput->GetKeysForAction(TEXT("ADS"));
	TArray <FInputActionKeyMapping> ActionFire = PlayerInput->GetKeysForAction(TEXT("Fire"));

	if (PlayerInput->InputKey(ActionADS[0].Key, EInputEvent::IE_Pressed, 1.0f, true)) {
		temp = NewObject<UADS_UpperState>(this, UADS_UpperState::StaticClass());
	}
	else if (PlayerInput->InputKey(ActionFire[0].Key, EInputEvent::IE_Pressed, 1.0f, true)){
		temp = NewObject<UAim_UpperState>(this, UAim_UpperState::StaticClass());
	}

	return temp;

}

UPlayerUpperStateBase* UArmed_UpperState::ChangeState(EPlayerUpperState State)
{
	return nullptr;
}

void UArmed_UpperState::StateStart_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UArmed_UpperState: StateStart"));
}

void UArmed_UpperState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
	
}

void UArmed_UpperState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UArmed_UpperState: StateEnd"));
}

UClass* UArmed_UpperState::GetStateClass_Implementation()
{
	return UArmed_UpperState::StaticClass();
}
