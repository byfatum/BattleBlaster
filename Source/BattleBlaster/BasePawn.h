#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();
	
protected:
	void RotateTurretTo(const FVector& TargetLocation, float DeltaTime) const;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> BaseComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> TurretComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UTurretAimingComponent> TurretAimingComponent;
};
