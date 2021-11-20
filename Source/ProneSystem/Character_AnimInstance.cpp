// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_AnimInstance.h"
#include "ProneSystemCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UCharacter_AnimInstance::UCharacter_AnimInstance()
{

}

void UCharacter_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		AProneSystemCharacter* Player = Cast<AProneSystemCharacter>(Pawn);

		TurnBodyYaw(Player, UpperYaw, UpperProneYaw);
		// ��Ʈ�ѷ��� ���� ������ pitch ���� ���Ѵ�.
		FRotator PitchAngle = (Player->GetCtrlRot() - Player->GetActorRotation()).GetNormalized();
		// �㸮�� 180���� ���°� �ƴϱ� ������ 90���� ������ �д�.
		UpperPitch = FMath::ClampAngle(PitchAngle.Pitch, -90.0f, 90.0f);
		
		PlayerSpeed = Player->GetVelocity().Size(); // �÷��̾� �ӵ�
		IsProne = Player->GetIsProne(); // ���帮�� ���� ����

		if (IsProne && Player->IsProneIK)
		{
			// Root
			FVector RootDir = ProneRootIK(Player, "pelvis", ProneIK_Pelvis_Rot, Player->IsShowCollision);

			// ô��
			FVector SpineDir = ProneMiddleIK(Player, "ProneSpineLoc", ProneIK_Upper_Rot, RootDir, Player->IsShowCollision);
			// ����
			FVector Calf_Rgiht_Dir = ProneMiddleIK(Player, "ProneCalfRightLoc", ProneIK_Right_Knee_Rot, RootDir, Player->IsShowCollision);
			FVector Calf_Left_Dir = ProneMiddleIK(Player, "ProneCalfLeftLoc", ProneIK_Left_Knee_Rot, RootDir, Player->IsShowCollision);

			// ��
			ProneEndIK(Player, "ProneFootRightLoc", ProneIK_Right_Foot_Rot, Calf_Rgiht_Dir, Player->IsShowCollision);
			ProneEndIK(Player, "ProneFootLeftLoc", ProneIK_Left_Foot_Rot, Calf_Left_Dir, Player->IsShowCollision);
			// �Ȳ�ġ
			ProneEndIK(Player, "ProneArmRightLoc", ProneIK_Right_Hand_Rot, SpineDir, Player->IsShowCollision);
			ProneEndIK(Player, "ProneArmLeftLoc", ProneIK_Left_Hand_Rot, SpineDir, Player->IsShowCollision);
		}
	}
	
}

void UCharacter_AnimInstance::TurnBodyYaw(AProneSystemCharacter* Player, float& Yaw, float& ProneYaw)
{
	float YawEnd = 0.0f;
	if (Player->GetIsProne()) // �÷��̾ ���帰�ٸ�
	{
		if (Player->GetVelocity().Size() > 3.0f)
		{
			// ������ �� ������ ����(TurnDirEnd)�� �����̴� �������� �ٲ۴�.
			TurnDirEnd = Player->GetActorRotation();

			if (!(ProneYaw <= 110.0f && ProneYaw >= -110.0f))
			{
				// ���� ���� �ٴ��� ���ϰ� �����ִٸ� ���ؾ� �Ǵ� ������ �ݴ�� �ϱ�
				TurnDirEnd = FRotator(TurnDirEnd.Pitch, TurnDirEnd.Yaw + 180.0f, TurnDirEnd.Roll);
				IsProneBack = true; // �����ִ� ����
			}
			else 
			{
				IsProneBack = false;  // ������ִ� ����
			}
		}
		else
		{
			// �������� ���� �� TurnDirEnd�� ActorRotation���� ���� ��Ų��.
			if (!IsTurn) // ���۵Ǵ� �������� ���� �ǰԲ� �ؾ� �ȴ�.
			{
				IsTurn = true;
				TurnDirEnd = Player->GetActorRotation();
			}
			if (!(ProneYaw <= 110.0f && ProneYaw >= -110.0f))
			{
				IsProneBack = true; // �����ִ� ����
			}
			else {
				IsProneBack = false; // ������ִ� ����
			}
		}

		// ���帮�� ���� �� �� �� �Ʒ��� �ٶ󺸸� �׿� ���� ��ü�� ������ �� �ְ� �ϴ� ���̴�.
		ProneRot = GetProneRotBlend(TurnDirEnd.Vector(), Player->GetActorRotation());
	}
	else {
		if (Player->GetVelocity().X > 0.0f || Player->GetVelocity().Y > 0.0f) 
		{ // ������ �� ��ü�� ���󰡰��Ѵ�.
			TurnDir = Player->GetActorRotation();
			TurnDirEnd = Player->GetActorRotation();
			IsTurn = false;
		}
		else 
		{ // �������� ���� �� ��ü�� ���� �����̰� �Ѵ�.
			if (!IsTurn) {
				IsTurn = true;
				TurnDirEnd = Player->GetActorRotation();
			}
			else {
				if (Yaw >= 70.0f || Yaw <= -70.0f) {
					TurnDirEnd = Player->GetActorRotation();
				}
			}
		}
	}
	TurnDir = FMath::RInterpTo(TurnDir, TurnDirEnd, GetWorld()->GetDeltaSeconds(), 5.0f);
	FRotator interpToAngle = (Player->GetActorRotation() - TurnDir).GetNormalized();
	ProneYaw = interpToAngle.Yaw; // ���帮�� ������ �� �� ��ü�� ȸ�� ������ 180�� ���� �޾ƾ� �Ѵ�.
	YawEnd = FMath::ClampAngle(interpToAngle.Yaw, -90.0f, 90.0f);
	Yaw = -YawEnd;

}

FVector UCharacter_AnimInstance::ProneRootIK(AProneSystemCharacter* Player, FName BoneName, FRotator& Rot, bool IsShow)
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

FVector UCharacter_AnimInstance::ProneMiddleIK(AProneSystemCharacter* Player, FName BoneName, FRotator& Rot, FVector Dir, bool IsShow)
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


void UCharacter_AnimInstance::ProneEndIK(AProneSystemCharacter* Player, FName BoneName, FRotator& Rot, FVector Dir, bool IsShow)
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

float UCharacter_AnimInstance::ProneDegrees(float ImpactNomalXY, float ImpactNomalZ, float DirXY, float DirZ, float XY)
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

FRotBlend UCharacter_AnimInstance::GetProneRotBlend(FVector PlayerForwardLoc, FRotator PlayerRot)
{
	PlayerForwardLoc.Normalize();
	FMatrix RotMatrix = FRotationMatrix(PlayerRot);
	FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
	FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
	FVector NormalizedVel = PlayerForwardLoc.GetSafeNormal2D();

	float DirForward = FVector::DotProduct(ForwardVector, NormalizedVel);
	float DirRight = FVector::DotProduct(RightVector, NormalizedVel);

	//UE_LOG(LogTemp, Warning, TEXT("DirForward: %f"), DirForward);
	//UE_LOG(LogTemp, Warning, TEXT("DirRight: %f"), DirRight);
	FRotBlend SetRot;
	SetRot.Front = FMath::Clamp(DirForward, 0.0f, 1.0f);
	SetRot.Back = FMath::Abs(FMath::Clamp(DirForward, -1.0f, 0.0f));
	SetRot.Left = FMath::Abs(FMath::Clamp(DirRight, -1.0f, 0.0f));
	SetRot.Right = FMath::Clamp(DirRight, 0.0f, 1.0f);

	return SetRot;
}
