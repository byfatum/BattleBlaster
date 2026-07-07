#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UCapsuleComponent;
class USceneComponent;
class UStaticMeshComponent;
class UTurretAimingComponent;

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();
	FVector GetAimTargetLocation() const;
	
protected:
	void RotateTurretTo(const FVector& TargetLocation, float DeltaTime) const;
	UStaticMeshComponent* GetTurretComponent() const;
	void Fire();

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> TurretComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UTurretAimingComponent> TurretAimingComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "AimTargetComponent")
	TObjectPtr<USceneComponent> AimTarget;
};
