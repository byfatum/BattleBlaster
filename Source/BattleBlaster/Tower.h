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
	void ReturnTurretToInitialRotation(float DeltaTime);
	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, Category = "TargetPawn")
	TWeakObjectPtr<ABasePawn> TargetPawn;
	
	UPROPERTY(VisibleAnywhere, Category = "DetectionRange")
	TObjectPtr<USphereComponent> DetectionSphere;
	
	UPROPERTY(VisibleAnywhere, Category = "DetectionRange")
	bool bIsTargetInRange = false;
	
	UPROPERTY(VisibleAnywhere, Category = "SightOrigin")
	TObjectPtr<USceneComponent> SightOrigin;
	
	UPROPERTY(EditDefaultsOnly, Category = "ReturnDelay")
	float ReturnDelay = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "ReturnRotationSpeed")
	float ReturnRotationSpeed = 100.0f;
	
	FRotator InitialTurretRotation = FRotator::ZeroRotator;
	float TimeWithoutTarget = 0.0f;
};
