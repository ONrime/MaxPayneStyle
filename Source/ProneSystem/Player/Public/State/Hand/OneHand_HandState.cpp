// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Hand/OneHand_HandState.h"
#include "Player/Public/State/Hand/TwoHand_HandState.h"
#include "Player/Public/State/Hand/BothHand_HandState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UOneHand_HandState::UOneHand_HandState()
{
    HandState = EPlayerHandState::ONEHAND;
}

UPlayerHandStateBase* UOneHand_HandState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionTwoHandWeapon = PlayerInput->GetKeysForAction(TEXT("TwoHandWeapon"));
	TArray <FInputActionKeyMapping> ActionBothWeapon = PlayerInput->GetKeysForAction(TEXT("BothWeapon"));

	if (PlayerInput->IsPressed(ActionTwoHandWeapon[0].Key))
	{
		temp = NewObject<UTwoHand_HandState>(this, UTwoHand_HandState::StaticClass());
	}
	else if (PlayerInput->IsPressed(ActionBothWeapon[0].Key)) {
		temp = NewObject<UBothHand_HandState>(this, UBothHand_HandState::StaticClass());
	}

	return temp;
}

UPlayerHandStateBase* UOneHand_HandState::ChangeState(EPlayerHandState State)
{
	if (State == EPlayerHandState::TWOHAND)
	{
		temp = NewObject<UTwoHand_HandState>(this, UTwoHand_HandState::StaticClass());
	}
	else if (State == EPlayerHandState::BOTHHAND) {
		temp = NewObject<UBothHand_HandState>(this, UBothHand_HandState::StaticClass());
	}

	return temp;
}


void UOneHand_HandState::StateStart_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UOneHand_HandState: StateStart"));

}

void UOneHand_HandState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UOneHand_HandState::StateEnd_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UOneHand_HandState: StateEnd"));

}

void UOneHand_HandState::PlayerFire(APlayerCharacter* Player)
{
}

void UOneHand_HandState::PlayerReload(APlayerCharacter* Player)
{
}

UClass* UOneHand_HandState::GetStateClass_Implementation()
{
    return UOneHand_HandState::StaticClass();
}
