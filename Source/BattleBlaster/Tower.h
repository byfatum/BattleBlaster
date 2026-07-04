#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "Tower.generated.h"

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
	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, Category = "TargetPawn")
	TWeakObjectPtr<ABasePawn> TargetPawn;
	
	UPROPERTY(VisibleAnywhere, Category = "DetectionRange")
	TObjectPtr<class USphereComponent> DetectionSphere;
	
	UPROPERTY(VisibleAnywhere, Category = "DetectionRange")
	bool bIsTargetInRange = false;
};
