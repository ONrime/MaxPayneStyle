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
	TArray <FInputActionKeyMapping> ActionProne = PlayerInput->GetKeysForAction(TEXT("Prone"));

	if (PlayerInput->IsPressed(ActionADS[0].Key)) {
		temp = NewObject<UADS_UpperState>(this, UADS_UpperState::StaticClass());
	}
	else if (PlayerInput->IsPressed(ActionFire[0].Key) || PlayerInput->IsPressed(ActionProne[0].Key)){
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

	// 카메라
	// 좌우 조절
	SpringArmLoc = FVector(0.0f, 0.0f, 0.0f);
	StartArmLength = 300.0f;

	Player->UpperSpringArmLoc.X = SpringArmLoc.X;
	Player->UpperSpringArmLoc.Y = SpringArmLoc.Y;
	Player->UpperSpringArmLoc.Z = SpringArmLoc.Z;
}

void UArmed_UpperState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
	// 카메라
	FRotator InterpToAngle = (Player->GetControlRotation() - Player->GetActorRotation()).GetNormalized();
	float CtrlPitch = FMath::ClampAngle(InterpToAngle.Pitch, -90.0f, 90.0f);

	if (CtrlPitch >= 0.0f)
	{
		ArmLength = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, Player->PitchUpLimite), FVector2D(StartArmLength, 30.0f), CtrlPitch);
	}
	else {
		ArmLength = FMath::GetMappedRangeValueClamped(FVector2D(-0.0f, Player->PitchDownLimite), FVector2D(StartArmLength, 150.0f), CtrlPitch);
		Player->UpperSpringArmLoc.X = FMath::GetMappedRangeValueClamped(FVector2D(-0.0f, Player->PitchDownLimite), FVector2D(SpringArmLoc.X, 20.0f), CtrlPitch);
		Player->UpperSpringArmLoc.Z = FMath::GetMappedRangeValueClamped(FVector2D(-0.0f, Player->PitchDownLimite), FVector2D(SpringArmLoc.Z, -25.0f), CtrlPitch);
	}

	Player->SpringArm->TargetArmLength = FMath::FInterpTo(Player->SpringArm->TargetArmLength, ArmLength, DeltaSecond, 4.0f);
	//UE_LOG(LogTemp, Warning, TEXT("Player->UpperSpringArmLoc.X: %f"), Player->UpperSpringArmLoc.X);
	//UE_LOG(LogTemp, Warning, TEXT("Player->UpperSpringArmLoc.Z: %f"), Player->UpperSpringArmLoc.Z);
	//UE_LOG(LogTemp, Warning, TEXT("CtrlPitch: %f"), CtrlPitch);
	//UE_LOG(LogTemp, Warning, TEXT("TargetArmLength: %f"), Player->SpringArm->TargetArmLength);
}

void UArmed_UpperState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UArmed_UpperState: StateEnd"));
}

void UArmed_UpperState::AnimInsUpdate(APlayerCharacter* Player, float& RootYaw, float& AimYaw, float& UpperYaw)
{
}

void UArmed_UpperState::PlayerFire(APlayerCharacter* Player)
{
	Super::PlayerFire(Player);
}

UClass* UArmed_UpperState::GetStateClass_Implementation()
{
	return UArmed_UpperState::StaticClass();
}
