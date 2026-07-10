#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 10.0f;
	
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraRotationLagSpeed = 10.0f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATank::BeginPlay() 
{
	Super::BeginPlay();
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const APlayerController* const PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;
	
	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult))
	{
		const FVector AimWorldLocation = HitResult.ImpactPoint;
	
		DrawDebugSphere(GetWorld(), AimWorldLocation, SphereRadius, SphereSegments, FColor::Green);
		this->RotateTurretTo(AimWorldLocation, DeltaTime);
	}
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	SetupInputMappingContext();
	
	UEnhancedInputComponent* const EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);	
	if (!EnhancedInput) return;

	if (!MoveAction) return;
	EnhancedInput->BindAction(MoveAction.Get(), ETriggerEvent::Triggered, this, &ATank::MoveInput);
	
	if (!TurnAction) return;
	EnhancedInput->BindAction(TurnAction.Get(), ETriggerEvent::Triggered, this, &ATank::TurnInput);
	
	if (!FireAction) return;
	EnhancedInput->BindAction(FireAction.Get(), ETriggerEvent::Started, this, &ATank::Fire);
}

void ATank::SetupInputMappingContext() 
{
	const APlayerController* const PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	const ULocalPlayer* const LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer) return;

	UEnhancedInputLocalPlayerSubsystem* const EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!EnhancedInputSubsystem) return;

	if (!DefaultMappingContext) return;
	EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
}

void ATank::MoveInput(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	const FVector DeltaLocation(InputValue * MoveSpeed * DeltaTime, 0.0,0.0);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::TurnInput(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	const FRotator DeltaRotation(0.0, InputValue * TurnRate * DeltaTime, 0.0);
	AddActorLocalRotation(DeltaRotation, true);
}