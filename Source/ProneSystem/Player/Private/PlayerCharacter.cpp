// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Public/PlayerCharacter.h"
#include "Player/Public/State/Upper/Armed_UpperState.h"
#include "Player/Public/State/Upper/UnArmed_UpperState.h"
#include "Player/Public/State/Lower/Standing_LowerState.h"
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
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;

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
	//static ConstructorHelpers::FClassFinder<UAnimInstance>FULLBODY_ANIMBP(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));
	//if (FULLBODY_ANIMBP.Succeeded()) { GetMesh()->SetAnimInstanceClass(FULLBODY_ANIMBP.Class); }

	CameraArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraArrow"));
	CameraArrow->SetupAttachment(RootComponent);
	CameraArrow->ArrowColor = FColor(255, 0, 0, 255);
	CameraArrow->bHiddenInGame = false;

	MoveInputArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MoveInputArrow"));
	MoveInputArrow->SetupAttachment(RootComponent);
	MoveInputArrow->ArrowColor = FColor(0, 255, 0, 255);
	MoveInputArrow->bHiddenInGame = false;

	BodyArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BodyArrow"));
	BodyArrow->SetupAttachment(RootComponent);
	BodyArrow->ArrowColor = FColor(0, 0, 255, 255);
	BodyArrow->bHiddenInGame = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UpperState = NewObject<UArmed_UpperState>(this, UArmed_UpperState::StaticClass());
	LowerState = NewObject<UStanding_LowerState>(this, UStanding_LowerState::StaticClass());
	UpperState->StateStart(this);
	LowerState->StateStart(this);

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpperState->StateUpdate(this, DeltaTime);
	LowerState->StateUpdate(this, DeltaTime);

	PlayerMove(IsMove, InputForwardDir, InputRightDir, MoveDir, PlayerSpeed, DeltaTime);

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
		LowerStateNowClass = LowerState->GetStateClass();
		LowerState->StateStart(this);
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
	AddControllerPitchInput(PitchRate);
}

void APlayerCharacter::PlayerProne()
{
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindAction("Prone", IE_Released, this, &APlayerCharacter::PlayerProne);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

