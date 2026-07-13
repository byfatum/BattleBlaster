#include "Tower.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SightOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("SightOrigin"));
	SightOrigin->SetupAttachment(this->GetTurretComponent());
	
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(GetRootComponent());
	
	DetectionSphere->SetGenerateOverlapEvents(true);
	DetectionSphere->SetCollisionProfileName(TEXT("TowerDetection"));
	DetectionSphere->SetSphereRadius(1000.0f);
	
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnSphereBeginOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ATower::OnSphereEndOverlap);
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	TargetPawn = FindTargetPawn();
	if (TargetPawn.IsValid())
	{
		bIsTargetInRange = DetectionSphere->IsOverlappingActor(TargetPawn.Get());
	}
	
	InitialTurretRotation = this->GetTurretComponent()->GetRelativeRotation();
	
	this->RegisterPawn();
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CanTrackTarget())
	{
		NoTargetElapsedTime = 0.0f;
		this->RotateTurretTo(TargetPawn.Get()->GetAimTargetLocation(), DeltaTime);
		
		if (IsAlignedToAimTarget())
		{
			AimLockElapsedTime = FMath::Min(AimLockElapsedTime + DeltaTime, AimLockDuration);
			
			if (AimLockElapsedTime >= AimLockDuration)
			{
				this->Fire();
				AimLockElapsedTime = 0.0f;
			}
		} 
		else
		{
			AimLockElapsedTime = 0.0f;
		}
	}
	else
	{
		NoTargetElapsedTime = FMath::Min(NoTargetElapsedTime + DeltaTime, ReturnDelay);
		AimLockElapsedTime = 0.0f;
		
		if (NoTargetElapsedTime >= ReturnDelay)
		{
			ReturnTurretToInitialRotation(DeltaTime);
		}
	}
}

ABasePawn* ATower::FindTargetPawn() const
{
	const APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return nullptr;
	
	APawn* const PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn) return nullptr;
	
	return Cast<ABasePawn>(PlayerPawn);
}

bool ATower::HasLineOfSightToTarget(const ABasePawn* const Target) const
{
	const FCollisionQueryParams Params(SCENE_QUERY_STAT(TowerLineOfSight), false, this);
	FHitResult OutHit;
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit, 
		SightOrigin->GetComponentLocation(), 
		Target->GetAimTargetLocation(), 
		ECC_GameTraceChannel2,
		Params);
	
	const bool bHasLineOfSight = bHit && OutHit.GetActor() == Target;
	
	DrawDebugLine(
		GetWorld(), 
		SightOrigin->GetComponentLocation(), 
		Target->GetAimTargetLocation(), 
		bHasLineOfSight ? FColor::Green : FColor::Red, 
		false, 
		0.1f);
	
	return bHasLineOfSight;
}

bool ATower::CanTrackTarget() const
{
	const ABasePawn* const Target = TargetPawn.Get();
	
	return Target && !Target->IsPawnDead() && bIsTargetInRange && HasLineOfSightToTarget(Target);
}

bool ATower::IsAlignedToAimTarget() const
{
	const FTransform MuzzleTransform = GetTurretComponent()->GetSocketTransform("Muzzle");
	
	const FVector2D MuzzleXYLocation(MuzzleTransform.GetLocation());
	const FVector2D TargetXYLocation(TargetPawn.Get()->GetAimTargetLocation());
	
	const FVector2D DirectionXYToTarget = (TargetXYLocation - MuzzleXYLocation).GetSafeNormal();
	const FVector2D MuzzleForwardXY(MuzzleTransform.GetUnitAxis(EAxis::X));
	
	const float Dot = FVector2D::DotProduct(DirectionXYToTarget, MuzzleForwardXY);
	
	return Dot >= FMath::Cos(FMath::DegreesToRadians(AimToleranceDegrees));
}

void ATower::ReturnTurretToInitialRotation(float DeltaTime)
{
	const FRotator NewRotation = FMath::RInterpConstantTo(
				this->GetTurretComponent()->GetRelativeRotation(), 
				InitialTurretRotation,
				DeltaTime,
				ReturnRotationSpeed);
			
	this->GetTurretComponent()->SetRelativeRotation(NewRotation, false);
}

void ATower::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor == TargetPawn.Get())
	{
		bIsTargetInRange = true;
	}
}

void ATower::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (OtherActor == TargetPawn.Get())
	{
		bIsTargetInRange = false;
	}
}