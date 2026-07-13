#include "TurretAimingComponent.h"
#include "Components/SceneComponent.h"
#include "Math/UnrealMathUtility.h"

UTurretAimingComponent::UTurretAimingComponent()
{
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = false;
}

void UTurretAimingComponent::SetTurretComponent(USceneComponent* InTurretComponent)
{
	ControlledTurretComponent = InTurretComponent;
}

void UTurretAimingComponent::AimAt(const FVector& TargetLocation, float DeltaTime) 
{
	if (!ControlledTurretComponent || !IsActive()) return;
	
	FVector TurretWorldLocation = ControlledTurretComponent->GetComponentLocation();
	TurretWorldLocation.Z = TargetLocation.Z;
	
	const FVector Direction = TargetLocation - TurretWorldLocation;
	
	const FRotator CurrentRotation = ControlledTurretComponent->GetComponentRotation();
	const FRotator TargetRotation = Direction.GetSafeNormal().Rotation();
	
	const FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, InterpTurretSpeed);
	
	ControlledTurretComponent->SetWorldRotation(NewRotation, false);
}