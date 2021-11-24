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

		//GetMoveDirBlend(Player->GetVelocity(), Player->GetActorRotation(), MoveDirBlend); // ADS
		//GetMoveDirBlend(Player->GetVelocity(), Player->GetMoveDir().Rotation(), MoveDirBlend);
		GetMoveDirBlend(Player->GetVelocity(), Player->GetBodyDir().Rotation(), MoveDirBlend); // Aim, Armed

		switch (Player->GetUpperStateNowEnum())
		{
		case EPlayerUpperState::ARMED:
			ArmedBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
			break;
		case EPlayerUpperState::AIM:
			AimBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
			break;
		case EPlayerUpperState::ADS:
			ADSBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
			break;
		}
		//ArmedBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
		//AimBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
		//ADSBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
		
		//UE_LOG(LogTemp, Warning, TEXT("RootYaw: %f"), RootYaw);
		///UE_LOG(LogTemp, Warning, TEXT("AimYaw: %f"), AimYaw);
		//UE_LOG(LogTemp, Warning, TEXT("UpperYaw: %f"), UpperYaw);
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
	float RootEnd = 0.0f;
	float RootRotInterpSpeedEnd = 3.0f;
	FRotator InterpToBodyAngle = (Player->GetBodyDir().Rotation() - Player->GetActorRotation()).GetNormalized();

	if (Player->GetVelocity().Size() > 0.3f)
	{
		IsTurn = false;
		TurnDir = Player->GetActorRotation();
		TurnDirEnd = Player->GetActorRotation();
		RootEnd = InterpToBodyAngle.Yaw;
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
		//Upper = FMath::ClampAngle(InterpToTurnDirAngle.Yaw, -90.0f, 90.0f);
		Upper = InterpToTurnDirAngle.Yaw;
		RootEnd = -Upper;
		RootRotInterpSpeedEnd = 100.0f;
	}
	RootRotInterpSpeed = FMath::FInterpTo(RootRotInterpSpeed, RootRotInterpSpeedEnd, GetWorld()->GetDeltaSeconds(), 5.0f); // Aim 이동o
	Root = FMath::FInterpTo(Root, RootEnd, GetWorld()->GetDeltaSeconds(), RootRotInterpSpeed); // Aim 이동o
	Aim = Root;
}

void UPlayerBodyAnimInstance::AimBodyYaw(APlayerCharacter* Player, float& Root, float& Aim, float& Upper)
{
	float RootEnd = 0.0f;
	float RootRotInterpSpeedEnd = 3.0f;
	FRotator InterpToBodyAngle = (Player->GetBodyDir().Rotation() - Player->GetActorRotation()).GetNormalized();

	if (Player->GetVelocity().Size() > 0.3f)
	{
		IsTurn = false;
		TurnDir = Player->GetActorRotation();
		TurnDirEnd = Player->GetActorRotation();
		RootEnd = InterpToBodyAngle.Yaw;
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
		RootEnd = -Upper;
		RootRotInterpSpeedEnd = 100.0f;
	}
	RootRotInterpSpeed = FMath::FInterpTo(RootRotInterpSpeed, RootRotInterpSpeedEnd, GetWorld()->GetDeltaSeconds(), 5.0f); // Aim 이동o
	Root = FMath::FInterpTo(Root, RootEnd, GetWorld()->GetDeltaSeconds(), RootRotInterpSpeed); // Aim 이동o
	Aim = Root;
}

void UPlayerBodyAnimInstance::ADSBodyYaw(APlayerCharacter* Player, float& Root, float& Aim, float& Upper)
{
	float RootEnd = 0.0f;

	if (Player->GetVelocity().Size() > 0.3f)
	{
		IsTurn = false;
		//TurnDir = Player->GetActorRotation();
		//TurnDirEnd = Player->GetActorRotation();
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
		RootEnd = -Upper;

		Root = RootEnd; // Aim 이동o
	}
}
