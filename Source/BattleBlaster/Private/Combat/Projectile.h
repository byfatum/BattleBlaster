#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class BATTLEBLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void LifeSpanExpired() override;

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat | Damage")
	float ProjectileDamage = 25.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraComponent> TrailEffectComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> HitEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundBase> LaunchSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundBase> HitSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ExpirationEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundBase> ExpirationSound;
};