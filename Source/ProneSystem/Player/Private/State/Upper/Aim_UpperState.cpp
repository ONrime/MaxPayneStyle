// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Upper/Aim_UpperState.h"
#include "Player/Public/State/Upper/Armed_UpperState.h"
#include "Player/Public/State/Upper/ADS_UpperState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UAim_UpperState::UAim_UpperState()
{
    UpperState = EPlayerUpperState::AIM;
}

UPlayerUpperStateBase* UAim_UpperState::HandleInput(APlayerCharacter* Player)
{
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	UPlayerInput* PlayerInput = Cast<UPlayerInput>(PlayerController->PlayerInput);
	TArray <FInputActionKeyMapping> ActionADS = PlayerInput->GetKeysForAction(TEXT("ADS"));
	TArray <FInputActionKeyMapping> ActionProne = PlayerInput->GetKeysForAction(TEXT("Prone"));

	if (PlayerInput->IsPressed(ActionADS[0].Key)) {
		temp = NewObject<UADS_UpperState>(this, UADS_UpperState::StaticClass());
	}
	else if (PlayerInput->IsPressed(ActionProne[0].Key))
	{
		temp = NewObject<UAim_UpperState>(this, UAim_UpperState::StaticClass());
	}

	return temp;
}

UPlayerUpperStateBase* UAim_UpperState::ChangeState(EPlayerUpperState State)
{
	if (State == EPlayerUpperState::ARMED)
	{
		temp = NewObject<UArmed_UpperState>(this, UArmed_UpperState::StaticClass());
	}

    return temp;
}

void UAim_UpperState::StateStart_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UAim_UpperState: StateStart"));

	PlayerCharacter = Player;
	if (Player->GetLowerStateNowEnum() != EPlayerLowerState::PRONE)
	{
		GetWorld()->GetTimerManager().SetTimer(ArmedTimer, this, &UAim_UpperState::ChangeArmed, 5.0f, false);
	}

	// 카메라
	// 좌우 조절
	SpringArmLoc = FVector(-35.0f, 45.0f, -5.0f);
	StartArmLength = 300.0f;

	Player->UpperSpringArmLoc.X = SpringArmLoc.X;
	if (Player->GetLowerStateNowEnum() == EPlayerLowerState::PRONE)
	{
		Player->UpperSpringArmLoc.Y = 0.0f;
	}
	else {
		Player->UpperSpringArmLoc.Y = SpringArmLoc.Y;
	}
	Player->UpperSpringArmLoc.Z = SpringArmLoc.Z;

}

void UAim_UpperState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
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

void UAim_UpperState::StateEnd_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UAim_UpperState: StateEnd"));
}

void UAim_UpperState::AnimInsUpdate(APlayerCharacter* Player, float& RootYaw, float& AimYaw, float& UpperYaw)
{
}

void UAim_UpperState::PlayerFire(APlayerCharacter* Player)
{
	Super::PlayerFire(Player);

	if (Player->GetLowerStateNowEnum() != EPlayerLowerState::PRONE)
	{
		GetWorld()->GetTimerManager().ClearTimer(ArmedTimer);
		GetWorld()->GetTimerManager().SetTimer(ArmedTimer, this, &UAim_UpperState::ChangeArmed, 5.0f, false);
	}
}

UClass* UAim_UpperState::GetStateClass_Implementation()
{
    return UAim_UpperState::StaticClass();
}

void UAim_UpperState::ChangeArmed()
{
	PlayerCharacter->ChangeUpperStateCheck.Execute((int)EPlayerUpperState::ARMED);
}
