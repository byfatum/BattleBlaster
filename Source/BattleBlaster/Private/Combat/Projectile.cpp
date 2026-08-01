#include "Projectile.h"
#include "Pawns/BasePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Player/BattleBlasterPlayerController.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileComponent"));
	SetRootComponent(ProjectileMesh);
	
	ProjectileMesh->SetGenerateOverlapEvents(true);
	ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bShouldBounce = false;
	
	TrailEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffectComponent"));
	TrailEffectComponent->SetupAttachment(GetRootComponent());
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnComponentHit);
	
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, 
			LaunchSound, 
			GetActorLocation(),
			GetActorRotation()
		);
	}
}

void AProjectile::LifeSpanExpired()
{
	const FVector ExpirationLocation = GetActorLocation();
	const FRotator ExpirationRotation = GetActorRotation();

	if (ExpirationEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExpirationEffect,
			ExpirationLocation,
			ExpirationRotation
		);
	}

	if (ExpirationSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ExpirationSound,
			ExpirationLocation,
			ExpirationRotation
		);
	}
	
	Super::LifeSpanExpired();
}

void AProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
                                 UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const AActor* const ProjectileOwner = this->GetOwner();
	
	ABasePawn* const DamagedActor = Cast<ABasePawn>(OtherActor);
	
	if (DamagedActor && ProjectileOwner && ProjectileOwner != OtherActor)
	{
		UGameplayStatics::ApplyDamage(
			DamagedActor, 
			ProjectileDamage, 
			ProjectileOwner->GetInstigatorController(), 
			this,
			UDamageType::StaticClass()
		);
	}
	
	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), 
			HitEffect, 
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, 
			HitSound,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}
	
	if (ABattleBlasterPlayerController* const PlayerController = 
		Cast<ABattleBlasterPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PlayerController->StartHitCameraShake();
	}
	
	this->Destroy();
}
