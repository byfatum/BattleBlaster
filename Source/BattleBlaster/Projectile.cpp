#include "Projectile.h"
#include "BasePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileComponent"));
	SetRootComponent(ProjectileMesh);
	
	ProjectileMesh->SetGenerateOverlapEvents(true);
	ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bShouldBounce = false;
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnComponentHit);
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
	
	this->Destroy();
}
