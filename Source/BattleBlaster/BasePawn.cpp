#include "BasePawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "TurretAimingComponent.h"
#include "Engine/StaticMeshSocket.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	BaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseComponent"));
	BaseComponent->SetupAttachment(CapsuleComponent);

	TurretComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretComponent"));
	TurretComponent->SetupAttachment(BaseComponent);
	
	TurretAimingComponent = CreateDefaultSubobject<UTurretAimingComponent>(TEXT("TurretAimingComponent"));
	TurretAimingComponent->SetTurretComponent(TurretComponent);
	
	AimTarget = CreateDefaultSubobject<USceneComponent>(TEXT("AimTargetComponent"));
	AimTarget->SetupAttachment(TurretComponent);
}

FVector ABasePawn::GetAimTargetLocation() const
{
	return AimTarget ? AimTarget->GetComponentLocation() : FVector::ZeroVector;
}

void ABasePawn::RotateTurretTo(const FVector& TargetLocation, float DeltaTime) const
{
	if (!TurretAimingComponent) return;
	TurretAimingComponent->AimAt(TargetLocation, DeltaTime);
}

UStaticMeshComponent* ABasePawn::GetTurretComponent() const
{
	return TurretComponent.Get();
}

void ABasePawn::Fire()
{
	const FName MuzzleSocketName("Muzzle");
	
	if (TurretComponent->DoesSocketExist(MuzzleSocketName))
	{
		const FVector SocketLocation = TurretComponent->GetSocketLocation(MuzzleSocketName);
		
		DrawDebugSphere(
			GetWorld(), 
			SocketLocation, 
			16, 
			16, 
			FColor::Green, 
			false,
			1.0f
		);
	}
}