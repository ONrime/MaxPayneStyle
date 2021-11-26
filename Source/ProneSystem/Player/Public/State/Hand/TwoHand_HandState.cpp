// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Hand/TwoHand_HandState.h"
#include "Player/Public/State/Hand/OneHand_HandState.h"
#include "Player/Public/State/Hand/BothHand_HandState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UTwoHand_HandState::UTwoHand_HandState()
{
	HandState = EPlayerHandState::TWOHAND;
}

UPlayerHandStateBase* UTwoHand_HandState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionOneWeapon = PlayerInput->GetKeysForAction(TEXT("OneWeapon"));
	TArray <FInputActionKeyMapping> ActionTwoWeapon = PlayerInput->GetKeysForAction(TEXT("TwoWeapon"));

	if (PlayerInput->IsPressed(ActionOneWeapon[0].Key) || PlayerInput->IsPressed(ActionTwoWeapon[0].Key))
	{
		temp = NewObject<UOneHand_HandState>(this, UOneHand_HandState::StaticClass());
	}

	return temp;
}

UPlayerHandStateBase* UTwoHand_HandState::ChangeState(EPlayerHandState State)
{
	if (State == EPlayerHandState::ONEHAND)
	{
		temp = NewObject<UOneHand_HandState>(this, UOneHand_HandState::StaticClass());
	}
	else if (State == EPlayerHandState::BOTHHAND) {
		temp = NewObject<UBothHand_HandState>(this, UBothHand_HandState::StaticClass());
	}

	return temp;
}

void UTwoHand_HandState::StateStart_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UTwoHand_HandState: StateStart"));

}

void UTwoHand_HandState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UTwoHand_HandState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UTwoHand_HandState: StateEnd"));

}

void UTwoHand_HandState::PlayerFire(APlayerCharacter* Player)
{
}

void UTwoHand_HandState::PlayerReload(APlayerCharacter* Player)
{
}

UClass* UTwoHand_HandState::GetStateClass_Implementation()
{
	return UTwoHand_HandState::StaticClass();
}
