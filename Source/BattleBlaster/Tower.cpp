#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(GetRootComponent());
	
	DetectionSphere->SetGenerateOverlapEvents(true);
	DetectionSphere->SetSphereRadius(1000.0f);
	DetectionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnSphereBeginOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ATower::OnSphereEndOverlap);
}

void ATower::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor == TargetPawn)
	{
		bIsTargetInRange = true;
	}
}

void ATower::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (OtherActor == TargetPawn)
	{
		bIsTargetInRange = false;
	}
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	TargetPawn = FindTargetPawn();
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsTargetInRange)
	{
		if (const ABasePawn* const Target = TargetPawn.Get())
		{
			this->RotateTurretTo(Target->GetActorLocation(), DeltaTime);
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