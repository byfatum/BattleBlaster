#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

class USceneComponent;
class USphereComponent;

UCLASS()
class BATTLEBLASTER_API ATower : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATower();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	ABasePawn* FindTargetPawn() const;
	bool HasLineOfSightToTarget(const ABasePawn* const Target) const;
	bool CanTrackTarget() const;
	bool IsAlignedToAimTarget() const;
	void ReturnTurretToInitialRotation(float DeltaTime);
	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, Category = "Tracking")
	TWeakObjectPtr<ABasePawn> TargetPawn;
	
	UPROPERTY(VisibleAnywhere, Category = "Tracking")
	TObjectPtr<USphereComponent> DetectionSphere;
	
	UPROPERTY(VisibleAnywhere, Category = "Tracking")
	TObjectPtr<USceneComponent> SightOrigin;
	
	UPROPERTY(VisibleAnywhere, Category = "Tracking")
	bool bIsTargetInRange = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tracking")
	float ReturnDelay = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tracking")
	float ReturnRotationSpeed = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	float AimLockDuration = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	float AimToleranceDegrees = 10.0f;
	
	FRotator InitialTurretRotation = FRotator::ZeroRotator;
	float NoTargetElapsedTime = 0.0f;
	float AimLockElapsedTime = 0.0f;
};
