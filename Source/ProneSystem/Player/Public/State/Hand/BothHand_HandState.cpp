// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Hand/BothHand_HandState.h"
#include "Player/Public/State/Hand/OneHand_HandState.h"
#include "Player/Public/State/Hand/TwoHand_HandState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UBothHand_HandState::UBothHand_HandState()
{
	HandState = EPlayerHandState::BOTHHAND;
}

UPlayerHandStateBase* UBothHand_HandState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionTwoHandWeapon = PlayerInput->GetKeysForAction(TEXT("TwoHandWeapon"));
	TArray <FInputActionKeyMapping> ActionOneWeapon = PlayerInput->GetKeysForAction(TEXT("OneWeapon"));
	TArray <FInputActionKeyMapping> ActionTwoWeapon = PlayerInput->GetKeysForAction(TEXT("TwoWeapon"));

	if (PlayerInput->IsPressed(ActionTwoHandWeapon[0].Key))
	{
		temp = NewObject<UTwoHand_HandState>(this, UTwoHand_HandState::StaticClass());
	}
	else if (PlayerInput->IsPressed(ActionOneWeapon[0].Key) || PlayerInput->IsPressed(ActionTwoWeapon[0].Key))
	{
		temp = NewObject<UOneHand_HandState>(this, UOneHand_HandState::StaticClass());
	}

	return temp;
}

UPlayerHandStateBase* UBothHand_HandState::ChangeState(EPlayerHandState State)
{
	if (State == EPlayerHandState::TWOHAND)
	{
		temp = NewObject<UTwoHand_HandState>(this, UTwoHand_HandState::StaticClass());
	}
	else if (State == EPlayerHandState::ONEHAND)
	{
		temp = NewObject<UOneHand_HandState>(this, UOneHand_HandState::StaticClass());
	}

	return temp;
}

void UBothHand_HandState::StateStart_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UBothHand_HandState: StateStart"));

}

void UBothHand_HandState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UBothHand_HandState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UBothHand_HandState: StateEnd"));
}

void UBothHand_HandState::PlayerFire(APlayerCharacter* Player)
{
}

void UBothHand_HandState::PlayerReload(APlayerCharacter* Player)
{
}

UClass* UBothHand_HandState::GetStateClass_Implementation()
{
	return UBothHand_HandState::StaticClass();
}
