// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/State/Lower/Dodge_LowerState.h"
#include "Player/Public/State/Lower/Prone_LowerState.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UDodge_LowerState::UDodge_LowerState()
{
    LowerState = EPlayerLowerState::DODGE;
}

UPlayerLowerStateBase* UDodge_LowerState::HandleInput(APlayerCharacter* Player)
{
    return nullptr;
}

UPlayerLowerStateBase* UDodge_LowerState::ChangeState(EPlayerLowerState State)
{
	if (State == EPlayerLowerState::PRONE)
	{
		temp = NewObject<UProne_LowerState>(this, UProne_LowerState::StaticClass());
	}

	return temp;
}

void UDodge_LowerState::StateStart_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UDodge_LowerState: StateStart"));
	
	Player->SetProneRot(Player->GetMoveDir().Rotation());
	Player->IsDodge = true;
	
	PlayerCharacter = Player;
	Player->SetIsMove(false);

	GetWorld()->GetTimerManager().SetTimer(DodgeStartTimer, this, &UDodge_LowerState::StartDodge, 0.15f, false);
	GetWorld()->GetTimerManager().SetTimer(DodgeMiddleTimer, this, &UDodge_LowerState::MiddleDodge, 0.35f, false);
	//StartDodge();

	// 카메라
	 // 상하 조절
	Player->LowerSpringArmLoc.Y = 0.0f;
	Player->UpperSpringArmLoc.Y = 0.0f;
	Player->SpringArmLocSpeed = 5.0f;
	// 상하 시점 한계 지점 설정
	Player->PitchUpLimite = 60.0f;
	Player->PitchDownLimite = -30.0f;

	CurrentBulletTime = 0.2f;
}

void UDodge_LowerState::StateUpdate_Implementation(APlayerCharacter* Player, float DeltaSecond)
{
	if (IsDodgeStart)
	{
		if (Player->GetMovementComponent()->IsFalling() == false)
		{
			Player->ChangeLowerStateCheck.Execute((int)EPlayerLowerState::PRONE);
		}
	}

	if (IsDodgeMiddle && IsDodgeStart)
	{
		if (CurrentBulletTime > 0.32f)
		{
			CurrentBulletTime = FMath::FInterpTo(CurrentBulletTime, 1.0f, DeltaSecond, 12.0f);
		}
		else
		{
			CurrentBulletTime = FMath::FInterpTo(CurrentBulletTime, 1.0f, DeltaSecond, 0.35f);
		}
		Player->BulletTime(CurrentBulletTime);
		//Player->CustomTimeDilation = 3.0f*CurrentBulletTime;
	}
	Player->LowerSpringArmLoc.Z = FMath::FInterpTo(Player->LowerSpringArmLoc.Z, 0.0f, DeltaSecond, 3.0f);

	//UE_LOG(LogTemp, Warning, TEXT("CurrentBulletTime: %f"), CurrentBulletTime);
	//UE_LOG(LogTemp, Warning, TEXT("LowerSpringArmLoc.Z: %f"), Player->LowerSpringArmLoc.Z);
	//UE_LOG(LogTemp, Warning, TEXT("UpperSpringArmLoc.Z: %f"), Player->UpperSpringArmLoc.Z);
	//UE_LOG(LogTemp, Warning, TEXT("SpringArm.Z: %f"), Player->SpringArm->GetRelativeLocation().Z);
	//UE_LOG(LogTemp, Warning, TEXT("GetSocketLocation.Z: %f"), Player->GetMesh()->GetSocketLocation("root").Z);

}

void UDodge_LowerState::StateEnd_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("UDodge_LowerState: StateEnd"));

	Player->SetIsMove(true);
	IsDodgeStart = false;
	Player->LowerSpringArmLoc.Z = 0.0f;
	Player->BulletTime(1.0f);
	Player->CustomTimeDilation = 1.0f;
}

UClass* UDodge_LowerState::GetStateClass_Implementation()
{
    return UDodge_LowerState::StaticClass();
}

void UDodge_LowerState::StartDodge()
{
	FVector LaunchDir = FVector(PlayerCharacter->GetMoveDir().X * 700.0f, PlayerCharacter->GetMoveDir().Y * 700.0f, 410.0f);
	PlayerCharacter->LaunchCharacter(LaunchDir, false, false);
	PlayerCharacter->LowerSpringArmLoc.Z = 180.0f;
	IsDodgeStart = true;
}

void UDodge_LowerState::MiddleDodge()
{
	IsDodgeMiddle = true;
}
