#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "BasePawn.generated.h"

class ABattleBlasterGameMode;
class UCapsuleComponent;
class UHealthComponent;
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
	void BeginPlay() override;
	void RotateTurretTo(const FVector& TargetLocation, float DeltaTime) const;
	UStaticMeshComponent* GetTurretComponent() const;
	void Fire();
	void RegisterPawn();
	void PawnDied();

private:
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<ABattleBlasterGameMode> GameMode;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	TObjectPtr<UStaticMeshComponent> BaseComponent;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	TObjectPtr<UStaticMeshComponent> TurretComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Aiming")
	TObjectPtr<UTurretAimingComponent> TurretAimingComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Aiming")
	TObjectPtr<USceneComponent> AimTarget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat | Damage")
	TSubclassOf<AProjectile> ProjectileType;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat | Health")
	TObjectPtr<UHealthComponent> HealthComponent;
};
