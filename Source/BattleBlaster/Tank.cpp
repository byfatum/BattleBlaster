#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"

ATank::ATank()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATank::BeginPlay() 
{
	Super::BeginPlay();

	SetupInputMappingContext();
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* const EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);	
	if (!EnhancedInput) return;

	if (MoveAction)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
	}
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

	UE_LOG(LogTemp, Display, TEXT("Input Value is %f"), InputValue);
}