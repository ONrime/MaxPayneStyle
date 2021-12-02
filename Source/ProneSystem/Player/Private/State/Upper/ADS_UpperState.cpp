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

	// 카메라
	 // 좌우 조절
	SpringArmLoc = FVector(-35.0f, 45.0f, -5.0f);
	StartArmLength = 50.0f;

	Player->UpperSpringArmLoc.X = SpringArmLoc.X;
	Player->UpperSpringArmLoc.Y = SpringArmLoc.Y;
	Player->UpperSpringArmLoc.Z = SpringArmLoc.Z;

}

void UADS_UpperState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
	// 카메라
	FRotator InterpToAngle = (Player->GetControlRotation() - Player->GetActorRotation()).GetNormalized();
	float CtrlPitch = FMath::ClampAngle(InterpToAngle.Pitch, -90.0f, 90.0f);

	if (CtrlPitch >= 0.0f)
	{
		ArmLength = FMath::GetMappedRangeValueClamped(FVector2D(40.0f, Player->PitchUpLimite), FVector2D(StartArmLength, 30.0f), CtrlPitch);
	}
	else {
		ArmLength = StartArmLength;
		Player->UpperSpringArmLoc.X = FMath::GetMappedRangeValueClamped(FVector2D(-0.0f, Player->PitchDownLimite), FVector2D(SpringArmLoc.X, 0.0f), CtrlPitch);
		Player->UpperSpringArmLoc.Z = FMath::GetMappedRangeValueClamped(FVector2D(-0.0f, Player->PitchDownLimite), FVector2D(SpringArmLoc.Z, 15.0f), CtrlPitch);
	}
	Player->SpringArm->TargetArmLength = FMath::FInterpTo(Player->SpringArm->TargetArmLength, ArmLength, DeltaSecond, 4.0f);
	//UE_LOG(LogTemp, Warning, TEXT("Player->UpperSpringArmLoc.X: %f"), Player->UpperSpringArmLoc.X);
	//UE_LOG(LogTemp, Warning, TEXT("Player->UpperSpringArmLoc.Z: %f"), Player->UpperSpringArmLoc.Z);
	//UE_LOG(LogTemp, Warning, TEXT("CtrlPitch: %f"), CtrlPitch);
	//UE_LOG(LogTemp, Warning, TEXT("TargetArmLength: %f"), Player->SpringArm->TargetArmLength);

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
