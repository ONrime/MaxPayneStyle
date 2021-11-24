// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Upper/ADS_UpperState.h"
#include "Player/Public/State/Upper/Aim_UpperState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UADS_UpperState::UADS_UpperState()
{
	UpperState = EPlayerUpperState::ADS;
}

UPlayerUpperStateBase* UADS_UpperState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionADS = PlayerInput->GetKeysForAction(TEXT("ADS"));
	TArray <FInputActionKeyMapping> ActionFire = PlayerInput->GetKeysForAction(TEXT("Fire"));

	if (PlayerInput->IsPressed(ActionFire[0].Key)) {
		temp = nullptr;
	}
	else if (PlayerInput->InputKey(ActionADS[0].Key, EInputEvent::IE_Released, 1.0f, true)) {
		temp = NewObject<UAim_UpperState>(this, UAim_UpperState::StaticClass());
	}

	return temp;
}

UPlayerUpperStateBase* UADS_UpperState::ChangeState(EPlayerUpperState State)
{
	return nullptr;
}

void UADS_UpperState::StateStart_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UADS_UpperState: StateStart"));
}

void UADS_UpperState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UADS_UpperState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UADS_UpperState: StateEnd"));
}

void UADS_UpperState::AnimInsUpdate(APlayerCharacter* Player, float& RootYaw, float& AimYaw, float& UpperYaw)
{
}

void UADS_UpperState::PlayerFire(APlayerCharacter* Player)
{
	Super::PlayerFire(Player);
}

UClass* UADS_UpperState::GetStateClass_Implementation()
{
	return UADS_UpperState::StaticClass();
}
