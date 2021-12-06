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
		InputSpeed = Player->GetMoveDir().Size();
		//UE_LOG(LogTemp, Warning, TEXT("InputSpeed: %f"), InputSpeed);

		if (Player->IsProne)
		{
			ProneBodyYaw(Player, RootYaw, AimYaw, UpperYaw, Player->GetProneRot());
			// Root
			FVector RootDir = ProneRootIK(Player, "pelvis", ProneIK_Pelvis_Rot, false);
			// 척추
			FVector SpineDir = ProneMiddleIK(Player, "ProneSpineLoc", ProneIK_Upper_Rot, RootDir, false);
			// 무릎
			FVector Calf_Rgiht_Dir = ProneMiddleIK(Player, "ProneCalfRightLoc", ProneIK_Right_Knee_Rot, RootDir, false);
			FVector Calf_Left_Dir = ProneMiddleIK(Player, "ProneCalfLeftLoc", ProneIK_Left_Knee_Rot, RootDir, false);

			// 발
			ProneEndIK(Player, "ProneFootRightLoc", ProneIK_Right_Foot_Rot, Calf_Rgiht_Dir, false);
			ProneEndIK(Player, "ProneFootLeftLoc", ProneIK_Left_Foot_Rot, Calf_Left_Dir, false);
			// 팔꿈치
			ProneEndIK(Player, "ProneArmRightLoc", ProneIK_Right_Hand_Rot, SpineDir, false);
			ProneEndIK(Player, "ProneArmLeftLoc", ProneIK_Left_Hand_Rot, SpineDir, false);
		}
		else if (Player->IsDodge)
		{
			DodgeBodyYaw(Player, RootYaw, AimYaw, UpperYaw, Player->GetProneRot());
		}
		else {
			switch (Player->GetUpperStateNowEnum())
			{
			case EPlayerUpperState::ARMED:
				GetMoveDirBlend(Player->GetVelocity(), Player->GetBodyDir().Rotation(), MoveDirBlend); // Aim, Armed
				ArmedBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
				break;
			case EPlayerUpperState::AIM:
				switch (Player->GetHandStateNowEnum())
				{
				case EPlayerHandState::ONEHAND:
					GetMoveDirBlend(Player->GetVelocity(), Player->GetBodyDir().Rotation(), MoveDirBlend); // Aim, Armed
					AimBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
					break;
				case EPlayerHandState::TWOHAND:
					GetMoveDirBlend(Player->GetVelocity(), Player->GetActorRotation(), MoveDirBlend); // ADS
					ADSBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
					break;
				case EPlayerHandState::BOTHHAND:
					GetMoveDirBlend(Player->GetVelocity(), Player->GetActorRotation(), MoveDirBlend); // ADS
					ADSBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
					break;
				}
				break;
			case EPlayerUpperState::ADS:
				GetMoveDirBlend(Player->GetVelocity(), Player->GetActorRotation(), MoveDirBlend); // ADS
				ADSBodyYaw(Player, RootYaw, AimYaw, UpperYaw);
				break;
			}
		}
		
		//UE_LOG(LogTemp, Warning, TEXT("RootYaw: %f"), RootYaw);
		//UE_LOG(LogTemp, Warning, TEXT("AimYaw: %f"), AimYaw);
		//UE_LOG(LogTemp, Warning, TEXT("UpperYaw: %f"), UpperYaw);
		//UE_LOG(LogTemp, Warning, TEXT("Test: %f"), Test.Yaw);

		LowerStateNClass = Player->LowerStateNowClass;
		LowerStateBClass = Player->LowerStateBeforeClass;
		UpperStateNClass = Player->UpperStateNowClass;
		HandStateNClass = Player->HandStateNowClass;

		IsMove = Player->GetIsMove();
		IsJumped = Player->bPressedJump;
		IsFalling = Player->GetMovementComponent()->IsFalling();
		IsDodge = Player->IsDodge;
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
				if (Upper > 0.0f)
				{
					TurnStart = Upper/ Upper;
				}
				else if (Upper < 0.0f) {
					TurnStart = -(Upper / Upper);
				}
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

void UPlayerBodyAnimInstance::ProneBodyYaw(APlayerCharacter* Player, float& Root, float& Aim, float& Upper, FRotator DirEnd)
{
	float yawEnd = 0.0f;
	if (Player->GetMoveDir().Size() >= 2.0f)
	{
		// 움직일 때 고정된 방향(TurnDirEnd)을 움직이는 방향으로 바꾼다.
		TurnDirEnd = DirEnd;

		if (!(Aim <= 110.0f && Aim >= -110.0f))
		{
			// 만약 등이 바닥을 향하게 누워있다면 향해야 되는 방향을 반대로 하기
			TurnDirEnd = FRotator(TurnDirEnd.Pitch, TurnDirEnd.Yaw + 180.0f, TurnDirEnd.Roll);
			IsProneBack = true; // 누워있는 상태
		}
		else {
			IsProneBack = false;  // 엎드려있는 상태
		}
	}
	else
	{
		// 움직이지 않을 때 TurnDirEnd을 ActorRotation으로 고정 시킨다.
		if (!IsTurn) // 시작되는 순간에만 고정 되게끔 해야 된다.
		{
			IsTurn = true;
			TurnDirEnd = DirEnd;
		}
		if (!(Aim <= 110.0f && Aim >= -110.0f))
		{
			IsProneBack = true; // 누워있는 상태
		}
		else {
			IsProneBack = false; // 엎드려있는 상태
		}
	}
	TurnDir = FMath::RInterpTo(TurnDir, TurnDirEnd, GetWorld()->GetDeltaSeconds(), 5.0f);
	FRotator interpToAngle = (Player->GetActorRotation() - TurnDir).GetNormalized();
	Aim = interpToAngle.Yaw; // 엎드리기 상태일 때 는 전체를 회전 함으로 180도 값을 받아야 한다.
	yawEnd = FMath::ClampAngle(interpToAngle.Yaw, -90.0f, 90.0f);
	Root = 0.0f;

	ProneRot = GetProneRotBlend(TurnDirEnd.Vector(), Player->GetActorRotation());
}

void UPlayerBodyAnimInstance::DodgeBodyYaw(APlayerCharacter* Player, float& Root, float& Aim, float& Upper, FRotator DirEnd)
{
	// 움직이지 않을 때 TurnDirEnd을 ActorRotation으로 고정 시킨다.
	if (!IsTurn) // 시작되는 순간에만 고정 되게끔 해야 된다.
	{
		IsTurn = true;
		TurnDirEnd = DirEnd;
	}
	if (!(Aim <= 110.0f && Aim >= -110.0f))
	{
		IsProneBack = true; // 누워있는 상태
	}
	else {
		IsProneBack = false; // 엎드려있는 상태
	}
	TurnDir = FMath::RInterpTo(TurnDir, TurnDirEnd, GetWorld()->GetDeltaSeconds(), 5.0f);
	FRotator interpToAngle = (Player->GetActorRotation() - TurnDir).GetNormalized();
	Aim = interpToAngle.Yaw; // 엎드리기 상태일 때 는 전체를 회전 함으로 180도 값을 받아야 한다.
	Root = 0.0f;

	ProneRot = GetProneRotBlend(TurnDirEnd.Vector(), DirEnd);
}

FProneRotBlend UPlayerBodyAnimInstance::GetProneRotBlend(FVector PlayerForwardLoc, FRotator PlayerRot)
{
	PlayerForwardLoc.Normalize();
	FMatrix RotMatrix = FRotationMatrix(PlayerRot);
	FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
	FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
	FVector NormalizedVel = PlayerForwardLoc.GetSafeNormal2D();

	float ProneDirForward = FVector::DotProduct(ForwardVector, NormalizedVel);
	float ProneDirRight = FVector::DotProduct(RightVector, NormalizedVel);

	//UE_LOG(LogTemp, Warning, TEXT("DirForward: %f"), DirForward);
	//UE_LOG(LogTemp, Warning, TEXT("DirRight: %f"), DirRight);
	FProneRotBlend SetRot;
	SetRot.Front = FMath::Clamp(ProneDirForward, 0.0f, 1.0f);
	SetRot.Back = FMath::Abs(FMath::Clamp(ProneDirForward, -1.0f, 0.0f));
	SetRot.Left = FMath::Abs(FMath::Clamp(ProneDirRight, -1.0f, 0.0f));
	SetRot.Right = FMath::Clamp(ProneDirRight, 0.0f, 1.0f);

	return SetRot;
}

FVector UPlayerBodyAnimInstance::ProneRootIK(APlayerCharacter* Player, FName BoneName, FRotator& Rot, bool IsShow)
{
	FVector SocketLoc = Player->GetMesh()->GetSocketLocation(BoneName);
	FVector StartTracer = SocketLoc + FVector(0.0f, 0.0f, 20.0f);
	FVector EndTracer = SocketLoc - FVector(0.0f, 0.0f, 60.0f);
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;

	bool hitis = false;
	if (IsShow)
	{
		hitis = UKismetSystemLibrary::SphereTraceSingle(this, StartTracer, EndTracer, 3.0f,
			ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame, OutHit, true);
	}
	else
	{
		hitis = UKismetSystemLibrary::SphereTraceSingle(this, StartTracer, EndTracer, 3.0f,
			ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::None, OutHit, true);
	}

	if (hitis)
	{
		FVector ImpactNomal = OutHit.ImpactNormal;
		/*UKismetSystemLibrary::SphereTraceSingle(this, ImpactNomal * 40.0f + SocketLoc, ImpactNomal * -40.0f + SocketLoc, 3.0f,
			ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame, OutHit, true);*/
			//UE_LOG(LogTemp, Warning, TEXT("ImpactNomal: %f, %f, %f"), ImpactNomal.X, ImpactNomal.Y, ImpactNomal.Z);
		float ProneRotationRoll = FMath::RadiansToDegrees(FMath::Atan2(ImpactNomal.Y, ImpactNomal.Z));
		float ProneRotationPitch = FMath::RadiansToDegrees(FMath::Atan2(ImpactNomal.X, ImpactNomal.Z) * -1.0f);
		Rot = FMath::RInterpTo(Rot, FRotator(ProneRotationPitch, 0.0f, ProneRotationRoll), GetWorld()->GetDeltaSeconds(), 15.0f);
		return ImpactNomal;
	}

	return FVector::ZeroVector;
}

FVector UPlayerBodyAnimInstance::ProneMiddleIK(APlayerCharacter* Player, FName BoneName, FRotator& Rot, FVector Dir, bool IsShow)
{
	FVector SocketLoc = Player->GetMesh()->GetSocketLocation(BoneName);
	FVector StartTracer = SocketLoc + Dir * 20.0f;
	FVector EndTracer = SocketLoc - Dir * 60.0f;
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;

	bool hitis = false;
	if (IsShow)
	{
		hitis = UKismetSystemLibrary::SphereTraceSingle(this, StartTracer, EndTracer, 3.0f,
			ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame, OutHit, true);
	}
	else
	{
		hitis = UKismetSystemLibrary::SphereTraceSingle(this, StartTracer, EndTracer, 3.0f,
			ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::None, OutHit, true);
	}

	if (hitis)
	{
		FVector ImpactNomal = OutHit.ImpactNormal;
		float ProneRotationRoll = ProneDegrees(ImpactNomal.Y, ImpactNomal.Z, Dir.Y, Dir.Z, 1.0f);
		float ProneRotationPitch = ProneDegrees(ImpactNomal.X, ImpactNomal.Z, Dir.X, Dir.Z, -1.0f);
		Rot = FMath::RInterpTo(Rot, FRotator(ProneRotationPitch, 0.0f, ProneRotationRoll), GetWorld()->GetDeltaSeconds(), 15.0f);
		return ImpactNomal;
	}

	return FVector::ZeroVector;
}

void UPlayerBodyAnimInstance::ProneEndIK(APlayerCharacter* Player, FName BoneName, FRotator& Rot, FVector Dir, bool IsShow)
{
	FVector SocketLoc = Player->GetMesh()->GetSocketLocation(BoneName);
	FVector StartTracer = SocketLoc + Dir * 20.0f;
	FVector EndTracer = SocketLoc - Dir * 60.0f;

	FRotator Test = FRotator(0.0f, 0.0f, 5.0f);

	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;

	bool hitis = false;
	if (IsShow)
	{
		hitis = UKismetSystemLibrary::SphereTraceSingle(this, StartTracer, EndTracer, 3.0f,
			ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame, OutHit, true);
	}
	else
	{
		hitis = UKismetSystemLibrary::SphereTraceSingle(this, StartTracer, EndTracer, 3.0f,
			ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::None, OutHit, true);
	}

	if (hitis)
	{
		FVector ImpactNomal = OutHit.ImpactNormal;
		/*UKismetSystemLibrary::SphereTraceSingle(this, ImpactNomal * 60.0f + SocketLoc, ImpactNomal * -60.0f + SocketLoc, 3.0f,
			ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame, OutHit, true);*/
		float ProneRotationRoll = ProneDegrees(ImpactNomal.Y, ImpactNomal.Z, Dir.Y, Dir.Z, 1.0f);
		float ProneRotationPitch = ProneDegrees(ImpactNomal.X, ImpactNomal.Z, Dir.X, Dir.Z, -1.0f);
		Rot = FMath::RInterpTo(Rot, FRotator(ProneRotationPitch, 0.0f, ProneRotationRoll), GetWorld()->GetDeltaSeconds(), 15.0f);
	}
}

float UPlayerBodyAnimInstance::ProneDegrees(float ImpactNomalXY, float ImpactNomalZ, float DirXY, float DirZ, float XY)
{
	float Degrees = 0.0f;
	if (DirZ > ImpactNomalZ)
	{
		Degrees = FMath::RadiansToDegrees(FMath::Atan2(ImpactNomalXY, DirZ) * XY);
	}
	else if (DirZ < ImpactNomalZ)
	{
		Degrees = FMath::RadiansToDegrees(FMath::Atan2(DirXY, ImpactNomalZ) * -1.0f * XY);
	}

	return Degrees;
}
