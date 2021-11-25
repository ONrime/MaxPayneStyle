// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/PlayerBodyAnimInstance.h"
#include "Player/Public/State/Upper/PlayerUpperStateBase.h"
#include "Player/Public/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerBodyAnimInstance::UPlayerBodyAnimInstance()
{

}

void UPlayerBodyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		APlayerCharacter* Player = Cast<APlayerCharacter>(Pawn);

		//UpperYaw = TurnBodyYaw(Player, UpperProneYaw);
		FRotator InterpToAngle = (Player->GetControlRotation() - Player->GetActorRotation()).GetNormalized();
		UpperPitch = FMath::ClampAngle(InterpToAngle.Pitch, -90.0f, 90.0f);

		PlayerSpeed = Player->GetVelocity().Size();

		//GetMoveDirBlend(Player->GetVelocity(), Player->GetMoveDir().Rotation(), MoveDirBlend);
		//GetMoveDirBlend(Player->GetVelocity(), Player->GetBodyDir().Rotation(), MoveDirBlend); // Aim, Armed

		switch (Player->GetUpperStateNowEnum())
		{
		case EPlayerUpperState::ARMED:
			GetMoveDirBlend(Player->GetVelocity(), Player->GetBodyDir().Rotation(), MoveDirBlend); // Aim, Armed
			ArmedBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
			break;
		case EPlayerUpperState::AIM:
			GetMoveDirBlend(Player->GetVelocity(), Player->GetBodyDir().Rotation(), MoveDirBlend); // Aim, Armed
			AimBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
			break;
		case EPlayerUpperState::ADS:
			GetMoveDirBlend(Player->GetVelocity(), Player->GetActorRotation(), MoveDirBlend); // ADS
			ADSBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
			break;
		}
		//ArmedBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
		//AimBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
		//ADSBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
		
		UE_LOG(LogTemp, Warning, TEXT("RootYaw: %f"), RootYaw);
		UE_LOG(LogTemp, Warning, TEXT("AimYaw: %f"), AimYaw);
		UE_LOG(LogTemp, Warning, TEXT("UpperYaw: %f"), UpperYaw);
		//UE_LOG(LogTemp, Warning, TEXT("Test: %f"), Test.Yaw);

		LowerStateNClass = Player->LowerStateNowClass;
		UpperStateNClass = Player->UpperStateNowClass;

		IsJumped = Player->bPressedJump;
		IsFalling = Player->GetMovementComponent()->IsFalling();
	}
}

void UPlayerBodyAnimInstance::GetMoveDirBlend(FVector Velocity, FRotator Rot, FMoveDirBlend& Current)
{
	Velocity.Normalize();
	FMatrix RotMatrix = FRotationMatrix(Rot);
	FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
	FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
	FVector NormalizedVel = Velocity.GetSafeNormal2D();

	DirForward = FVector::DotProduct(ForwardVector, NormalizedVel);
	DirRight = FVector::DotProduct(RightVector, NormalizedVel);

	if (DirForward > 0.3f)
	{
		InputDir = EInpuDir::FRONT;
	}
	else if (DirForward < -0.3f) {
		InputDir = EInpuDir::BACK;
	}
	else {
		if (FMath::Sign(DirRight) == 1) {
			InputDir = EInpuDir::RIGHT;
		}
		else if (FMath::Sign(DirRight) == -1) {
			InputDir = EInpuDir::LEFT;
		}
	}

	FMoveDirBlend SetDir;
	SetDir.Front = FMath::Clamp(DirForward, 0.0f, 1.0f);
	SetDir.Back = FMath::Abs(FMath::Clamp(DirForward, -1.0f, 0.0f));
	SetDir.Left = FMath::Abs(FMath::Clamp(DirRight, -1.0f, 0.0f));
	SetDir.Right = FMath::Clamp(DirRight, 0.0f, 1.0f);

	Current.Front = FMath::FInterpTo(Current.Front, SetDir.Front, GetWorld()->GetDeltaSeconds(), 8.0f);
	Current.Back = FMath::FInterpTo(Current.Back, SetDir.Back, GetWorld()->GetDeltaSeconds(), 8.0f);
	Current.Left = FMath::FInterpTo(Current.Left, SetDir.Left, GetWorld()->GetDeltaSeconds(), 8.0f);
	Current.Right = FMath::FInterpTo(Current.Right, SetDir.Right, GetWorld()->GetDeltaSeconds(), 8.0f);

	/*UE_LOG(LogTemp, Warning, TEXT("Current.Front: %f"), Current.Front);
	UE_LOG(LogTemp, Warning, TEXT("Current.Back: %f"), Current.Back);
	UE_LOG(LogTemp, Warning, TEXT("Current.Left: %f"), Current.Left);
	UE_LOG(LogTemp, Warning, TEXT("Current.Right: %f"), Current.Right);*/
}

void UPlayerBodyAnimInstance::ArmedBodyYaw(APlayerCharacter* Player, float& Root, float& Aim, float& Upper)
{
	if (Player->GetVelocity().Size() > 0.3f)
	{
		IsTurn = false;
		TurnDir = Player->GetActorRotation();
		FRotator InterpToBodyAngle = (Player->GetBodyDir().Rotation() - Player->GetActorRotation()).GetNormalized();
		Root = FMath::FInterpTo(Root, InterpToBodyAngle.Yaw, GetWorld()->GetDeltaSeconds(), 3.0f);
	}
	else
	{
		if (!IsTurn)
		{
			IsTurn = true;
			TurnDirEnd = Player->GetActorRotation();
		}
		TurnDir = FMath::RInterpTo(TurnDir, TurnDirEnd, GetWorld()->GetDeltaSeconds(), 5.0f);
		FRotator InterpToTurnDirAngle = (Player->GetActorRotation() - TurnDir).GetNormalized();
		Upper = InterpToTurnDirAngle.Yaw;
		Root = -Upper;
	}
	Aim = Root;
}

void UPlayerBodyAnimInstance::AimBodyYaw(APlayerCharacter* Player, float& Root, float& Aim, float& Upper)
{
	float RootEnd = 0.0f;
	float RootRotInterpSpeedEnd = 3.0f;
	
	if (Player->GetVelocity().Size() > 0.3f)
	{
		IsTurn = false;
		TurnDir = Player->GetActorRotation();

		FRotator InterpToBodyAngle = (Player->GetBodyDir().Rotation() - Player->GetActorRotation()).GetNormalized();
		FRotator InterpToActorAngle = (Player->GetActorRotation() - Player->GetBodyDir().Rotation()).GetNormalized();
		Root = FMath::FInterpTo(Root, InterpToBodyAngle.Yaw, GetWorld()->GetDeltaSeconds(), 3.0f);
		Aim = InterpToActorAngle.Yaw;
	}
	else
	{
		if (!IsTurn)
		{
			IsTurn = true;
			TurnDirEnd = Player->GetActorRotation();
		}
		else
		{
			if (Upper >= 70.0f || Upper <= -70.0f)
			{
				TurnDirEnd = Player->GetActorRotation();
			}
		}
		TurnDir = FMath::RInterpTo(TurnDir, TurnDirEnd, GetWorld()->GetDeltaSeconds(), 5.0f);
		FRotator InterpToTurnDirAngle = (Player->GetActorRotation() - TurnDir).GetNormalized();
		Upper = FMath::ClampAngle(InterpToTurnDirAngle.Yaw, -90.0f, 90.0f);
		Root = -Upper;
		Aim = 0.0f;
	}
}

void UPlayerBodyAnimInstance::ADSBodyYaw(APlayerCharacter* Player, float& Root, float& Aim, float& Upper)
{
	if (Player->GetVelocity().Size() > 0.3f)
	{
		IsTurn = false;
		TurnDir = Player->GetActorRotation();
		TurnDirEnd = Player->GetActorRotation();

		Aim = Upper;
	}
	else
	{
		if (!IsTurn)
		{
			IsTurn = true;
			TurnDirEnd = Player->GetActorRotation();
		}
		else
		{
			if (Upper >= 70.0f || Upper <= -70.0f)
			{
				TurnDirEnd = Player->GetActorRotation();
			}
		}
	}
	TurnDir = FMath::RInterpTo(TurnDir, TurnDirEnd, GetWorld()->GetDeltaSeconds(), 5.0f);
	FRotator InterpToTurnDirAngle = (Player->GetActorRotation() - TurnDir).GetNormalized();
	Upper = FMath::ClampAngle(InterpToTurnDirAngle.Yaw, -90.0f, 90.0f);
	Root = -Upper;
}
