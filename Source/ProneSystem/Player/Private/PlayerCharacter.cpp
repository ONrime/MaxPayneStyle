// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/PlayerCharacter.h"
#include "Player/Public/State/Upper/Armed_UpperState.h"
#include "Player/Public/State/Upper/UnArmed_UpperState.h"
#include "Player/Public/State/Lower/Standing_LowerState.h"
#include "Player/Public/State/Upper/PlayerUpperStateBase.h"
#include "Player/Public/State/Lower/PlayerLowerStateBase.h"
#include "Player/Public/State/Hand/PlayerHandStateBase.h"
#include "Player/Public/State/Hand/OneHand_HandState.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(20.0f, 96.0f);
	RootComponent = GetCapsuleComponent();

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true; //
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 00.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0.0f, 60.0f, 80.0f));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetMesh()->SetupAttachment(RootComponent);
	//GetMesh()->SetIsReplicated(true);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>FULLBODY_SKELETALMESH(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (FULLBODY_SKELETALMESH.Succeeded()) { GetMesh()->SetSkeletalMesh(FULLBODY_SKELETALMESH.Object); }
	static ConstructorHelpers::FClassFinder<UAnimInstance>FULLBODY_ANIMBP(TEXT("AnimBlueprint'/Game/MaxFile/Anim/PlayerAnimBP.PlayerAnimBP_C'"));
	if (FULLBODY_ANIMBP.Succeeded()) { GetMesh()->SetAnimInstanceClass(FULLBODY_ANIMBP.Class); }

	CameraArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraArrow"));
	CameraArrow->SetupAttachment(RootComponent);
	CameraArrow->ArrowColor = FColor(255, 0, 0, 255);
	CameraArrow->bHiddenInGame = true;

	MoveInputArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MoveInputArrow"));
	MoveInputArrow->SetupAttachment(RootComponent);
	MoveInputArrow->ArrowColor = FColor(0, 255, 0, 255);
	MoveInputArrow->bHiddenInGame = true;

	BodyArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BodyArrow"));
	BodyArrow->SetupAttachment(RootComponent);
	BodyArrow->ArrowColor = FColor(0, 0, 255, 255);
	BodyArrow->bHiddenInGame = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UpperState = NewObject<UArmed_UpperState>(this, UArmed_UpperState::StaticClass());
	UpperStateNowEnum = UpperState->GetEState();
	UpperStateNowClass = UpperState->GetStateClass();
	LowerState = NewObject<UStanding_LowerState>(this, UStanding_LowerState::StaticClass());
	LowerStateNowEnum = LowerState->GetEState();
	LowerStateNowClass = LowerState->GetStateClass();
	HandState = NewObject<UOneHand_HandState>(this, UOneHand_HandState::StaticClass());
	HandStateNowEnum = HandState->GetEState();
	HandStateNowClass = HandState->GetStateClass();
	UpperState->StateStart(this);
	LowerState->StateStart(this);
	HandState->StateStart(this);

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ChangeUpperStateCheck.BindLambda([this](int Set)->void
	{
		UpperPress(UpperState->ChangeState((EPlayerUpperState)Set));
	});

	ChangeLowerStateCheck.BindLambda([this](int Set)->void
	{
		LowerPress(LowerState->ChangeState((EPlayerLowerState)Set));
	});

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpperState->StateUpdate(this, DeltaTime);
	LowerState->StateUpdate(this, DeltaTime);
	HandState->StateUpdate(this, DeltaTime);

	PlayerMove(IsMove, InputForwardDir, InputRightDir, MoveDir, PlayerSpeed, DeltaTime);

	// Ä«¸Þ¶ó
	//CurrentSpringArmLoc = FMath::VInterpTo(CurrentSpringArmLoc, LowerSpringArmLoc + UpperSpringArmLoc, DeltaTime, SpringArmLocSpeed);
	//SpringArm->SetRelativeLocation(CurrentSpringArmLoc);
	SpringArm->SetWorldLocation(FVector(1153.0f, 339.0f, 310.0f));

}

void APlayerCharacter::UpperPress(UPlayerUpperStateBase* State)
{
	UPlayerUpperStateBase* temp = nullptr;
	if (!State) {
		temp = UpperState->HandleInput(this);
		if (temp == nullptr) { return; }
	}
	else { temp = State; }
	UpperState->StateEnd(this);
	EPlayerUpperState Before = UpperState->GetEState();
	UpperStateBeforeClass = UpperState->GetStateClass();
	if (temp != nullptr) {
		UpperState->DestroyComponent();
		UpperState = temp;
		UpperState->SetEBeforeState(Before);
		UpperStateNowEnum = UpperState->GetEState();
		UpperStateNowClass = UpperState->GetStateClass();
		UpperState->StateStart(this);
	}
}
void APlayerCharacter::LowerPress(UPlayerLowerStateBase* State)
{
	UPlayerLowerStateBase* temp = nullptr;
	if (!State) {
		temp = LowerState->HandleInput(this);
		if (temp == nullptr) { return; }
	}
	else { temp = State; }
	LowerState->StateEnd(this);
	EPlayerLowerState Before = LowerState->GetEState();
	LowerStateBeforeClass = LowerState->GetStateClass();
	if (temp != nullptr) {
		LowerState->DestroyComponent();
		LowerState = temp;
		LowerState->SetEBeforeState(Before);
		LowerStateNowEnum = LowerState->GetEState();
		LowerStateNowClass = LowerState->GetStateClass();
		LowerState->StateStart(this);
	}
}

void APlayerCharacter::HandPress(UPlayerHandStateBase* State)
{
	UPlayerHandStateBase* temp = nullptr;
	if (!State) {
		temp = HandState->HandleInput(this);
		if (temp == nullptr) { return; }
	}
	else { temp = State; }
	HandState->StateEnd(this);
	LowerStateBeforeClass = HandState->GetStateClass();
	if (temp != nullptr) {
		HandState->DestroyComponent();
		HandState = temp;
		HandStateNowEnum = HandState->GetEState();
		HandStateNowClass = HandState->GetStateClass();
		HandState->StateStart(this);
	}
}

void APlayerCharacter::PlayerMove(bool IsAactive, float Forward, float Right, FVector& Dir, float Speed, float DeltaSecond)
{
	if (IsAactive) {
		if (Dir.IsZero()) {
			return;
		}
		Dir.Normalize();
		float InputDir = 0.0f;
		if (FMath::Abs(Forward) > FMath::Abs(Right)) {
			InputDir = FMath::Abs(Forward);
		}
		else { InputDir = FMath::Abs(Right); }
		//UE_LOG(LogTemp, Warning, TEXT("Input: %f"), FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, PlayerSpeed), InputDir));
		AddMovementInput(Dir, FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, Speed), InputDir) * DeltaSecond);
		//DownState->PlayerMove(this, inputDir, inputDirRight);
		MoveInputArrow->SetWorldRotation(Dir.Rotation());
		BodyDir = FMath::VInterpTo(BodyDir, MoveDir, DeltaSecond, 1.35f);
		BodyArrow->SetWorldRotation(BodyDir.Rotation());
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (IsMove) {
		InputForwardDir = Value;
		if ((Controller != NULL) && (Value != 0.0f)) {
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::X);
			Direction.Z = 0.0f;
			Direction.Normalize();
			MoveDir += Direction * FMath::Clamp(Value, -1.0f, 1.0f);
			//UE_LOG(LogTemp, Warning, TEXT("InputDirForward: %f"), InputDirForward);
		}
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (IsMove) {
		InputRightDir = Value;
		if ((Controller != NULL) && (Value != 0.0f)) {
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::Y);
			Direction.Z = 0.0f;
			Direction.Normalize();
			MoveDir += Direction * FMath::Clamp(Value, -1.0f, 1.0f);
		}
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	float PitchRate = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	FRotator InterpToAngle = (GetControlRotation() - GetActorRotation()).GetNormalized();
	float CtrlPitch = FMath::ClampAngle(InterpToAngle.Pitch, -90.0f, 90.0f);

	if (CtrlPitch <= PitchDownLimite && PitchRate > 0.0f) {
		PitchRate = 0.0f;
	}
	else if (CtrlPitch >= PitchUpLimite && PitchRate < 0.0f)
	{
		PitchRate = 0.0f;
	}
	AddControllerPitchInput(PitchRate);

	//ArmDistance = FMath::GetMappedRangeValueClamped(FVector2D(00.0f, PitchUpLimite), FVector2D(0.0f, 10.0f), CtrlPitch);
	//FVector tset = FollowCamera->GetForwardVector() * PitchRate * -10.0f + SpringArm->GetComponentLocation();

	//SpringArm->SetWorldLocation(tset);

}

void APlayerCharacter::PlayerProne()
{
	LowerPress();
	UpperPress();
}

void APlayerCharacter::PlayerCrouch()
{
	LowerPress();
}

void APlayerCharacter::PlayerDodge()
{
	LowerPress();
	UpperPress();
}

void APlayerCharacter::PlayerOne1()
{
	HandPress();
}
void APlayerCharacter::PlayerOne2()
{
	HandPress();
}
void APlayerCharacter::PlayerTwo()
{
	HandPress();
}
void APlayerCharacter::PlayerBoth()
{
	HandPress();
}

void APlayerCharacter::PlayerADS()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerADS"));
	UpperPress();
}

void APlayerCharacter::PlayerUnADS()
{
	UpperPress();
}

void APlayerCharacter::PlayerFire()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerFire"));
	UpperPress();
	UpperState->PlayerFire(this);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("ArmedState", IE_Pressed, this, &APlayerCharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::PlayerFire);
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &APlayerCharacter::PlayerADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &APlayerCharacter::PlayerUnADS);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::PlayerCrouch);
	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &APlayerCharacter::PlayerProne);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APlayerCharacter::PlayerDodge);

	PlayerInputComponent->BindAction("OneWeapon", IE_Pressed, this, &APlayerCharacter::PlayerOne1);
	PlayerInputComponent->BindAction("TwoWeapon", IE_Pressed, this, &APlayerCharacter::PlayerOne1);
	PlayerInputComponent->BindAction("TwoHandWeapon", IE_Pressed, this, &APlayerCharacter::PlayerTwo);
	PlayerInputComponent->BindAction("BothWeapon", IE_Pressed, this, &APlayerCharacter::PlayerBoth);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

void APlayerCharacter::BulletTime(float Set)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Set);
}

