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

	if (PlayerInput->IsPressed(ActionADS[0].Key)) {
		temp = NewObject<UADS_UpperState>(this, UADS_UpperState::StaticClass());
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
	GetWorld()->GetTimerManager().SetTimer(ArmedTimer, this, &UAim_UpperState::ChangeArmed, 5.0f, false);

}

void UAim_UpperState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
}

void UAim_UpperState::StateEnd_Implementation(APlayerCharacter* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("UAim_UpperState: StateEnd"));
}

void UAim_UpperState::AnimInsUpdate(APlayerCharacter* Player, float& RootYaw, float& AimYaw, float& UpperYaw)
{
	FVector MoveDir = Player->GetMoveDir();
	FRotator InterpToAngle = (Player->GetBodyDir().Rotation() - Player->GetActorRotation()).GetNormalized();

	if (Player->GetVelocity().Size() > 0.3f)
	{
		RootYaw = FMath::FInterpTo(RootYaw, InterpToAngle.Yaw, GetWorld()->GetDeltaSeconds(), 3.0f); // Aim 이동o
	}
	else 
	{
		RootYaw = InterpToAngle.Yaw; // Aim 이동x
		UpperYaw = FMath::ClampAngle(InterpToAngle.Yaw, -90.0f, 90.0f);
	}
	//AimYaw = FMath::FInterpTo(RootYaw, InterpToAngle.Yaw, GetWorld()->GetDeltaSeconds(), 10.0f);
	AimYaw = RootYaw;
	
}

void UAim_UpperState::PlayerFire(APlayerCharacter* Player)
{
	Super::PlayerFire(Player);

	GetWorld()->GetTimerManager().ClearTimer(ArmedTimer);
	GetWorld()->GetTimerManager().SetTimer(ArmedTimer, this, &UAim_UpperState::ChangeArmed, 5.0f, false);

}

UClass* UAim_UpperState::GetStateClass_Implementation()
{
    return UAim_UpperState::StaticClass();
}

void UAim_UpperState::ChangeArmed()
{
	PlayerCharacter->ChangeStateCheck.Execute((int)EPlayerUpperState::ARMED);
}
