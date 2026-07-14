#include "BasePawn.h"

#include "BattleBlasterGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "TurretAimingComponent.h"
#include "Kismet/GameplayStatics.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->SetCollisionProfileName(TEXT("MovementBody"));

	BaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseComponent"));
	BaseComponent->SetupAttachment(CapsuleComponent);
	BaseComponent->SetGenerateOverlapEvents(false);
	BaseComponent->SetCollisionProfileName(TEXT("CombatHitbox"));

	TurretComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretComponent"));
	TurretComponent->SetupAttachment(BaseComponent);
	TurretComponent->SetGenerateOverlapEvents(false);
	TurretComponent->SetCollisionProfileName(TEXT("CombatHitbox"));
	
	TurretAimingComponent = CreateDefaultSubobject<UTurretAimingComponent>(TEXT("TurretAimingComponent"));
	TurretAimingComponent->SetTurretComponent(TurretComponent);
	
	AimTarget = CreateDefaultSubobject<USceneComponent>(TEXT("AimTargetComponent"));
	AimTarget->SetupAttachment(TurretComponent);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

FVector ABasePawn::GetAimTargetLocation() const
{
	return AimTarget ? AimTarget->GetComponentLocation() : FVector::ZeroVector;
}

bool ABasePawn::IsPawnDead() const
{
	return HealthComponent->IsDead();
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<ABattleBlasterGameMode>(UGameplayStatics::GetGameMode(this));
	HealthComponent->OnDeath().AddUObject(this, &ABasePawn::PawnDied);
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
		const FRotator SocketRotation = TurretComponent->GetSocketRotation(MuzzleSocketName);
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = this;
		
		const AProjectile* const Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileType, 
			SocketLocation, 
			SocketRotation,
			SpawnParameters
			);

		const APawn* const ProjectileInstigator = Projectile->GetInstigator();
		
		if (bShowDebugSphereDuringFire)
		{
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
}

void ABasePawn::RegisterPawn()
{
	if (GameMode.Get())
	{
		GameMode->RegisterPawn(this);
	}
}

void ABasePawn::PawnDied()
{
	if (GameMode.Get())
	{
		GameMode->ActorDied(this);
	}
	
	HandleDeath();
}

void ABasePawn::ActivateTurretAiming()
{
	if (!TurretAimingComponent) return;
	TurretAimingComponent->Activate();
}

void ABasePawn::DisableTurretAiming()
{
	if (!TurretAimingComponent) return;
	TurretAimingComponent->Deactivate();
}

void ABasePawn::SetPawnVisibility(const bool bNewVisibility)
{
	if (!BaseComponent.Get() || !TurretComponent.Get()) return;
	
	BaseComponent->SetVisibility(bNewVisibility);
	TurretComponent->SetVisibility(bNewVisibility);
}

void ABasePawn::SetPawnNoCollision()
{
	if (!CapsuleComponent.Get() || !BaseComponent.Get() || !TurretComponent.Get()) return;
	
	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));
	BaseComponent->SetCollisionProfileName(TEXT("NoCollision"));
	TurretComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABasePawn::HandleDeath()
{
	Destroy();
}
