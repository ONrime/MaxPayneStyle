// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/PlayerBodyAnimInstance.h"
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
		FVector MoveDir = Player->GetMoveDir();
		FRotator Test = (Player->GetBodyDir().Rotation() - Player->GetControlRotation()).GetNormalized();
		RootYaw = FMath::FInterpTo(RootYaw, Test.Yaw, GetWorld()->GetDeltaSeconds(), 3.0f);
		//RootYaw = FMath::ClampAngle(Test.Yaw, -170.0f, 170.0f);
		//float YawClamp = FMath::GetMappedRangeValueClamped(FVector2D(-180.0f, 180.0f), FVector2D(0.0f, 1.0f), Test.Yaw);
		//RootYaw = FMath::Lerp(-180.0f, 180.0f, YawClamp);
		GetMoveDirBlend(Player->GetVelocity(), Player->GetBodyDir().Rotation(), MoveDirBlend);
		
		UE_LOG(LogTemp, Warning, TEXT("RootYaw: %f"), RootYaw);
		UE_LOG(LogTemp, Warning, TEXT("Test: %f"), Test.Yaw);

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

	UE_LOG(LogTemp, Warning, TEXT("Current.Front: %f"), Current.Front);
	UE_LOG(LogTemp, Warning, TEXT("Current.Back: %f"), Current.Back);
	UE_LOG(LogTemp, Warning, TEXT("Current.Left: %f"), Current.Left);
	UE_LOG(LogTemp, Warning, TEXT("Current.Right: %f"), Current.Right);
}
