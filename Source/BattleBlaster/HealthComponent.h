#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEBLASTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	
	DECLARE_EVENT(UHealthComponent, FOnDeathSignature)
	FOnDeathSignature& OnDeath();
	bool IsDead() const;

protected:
	virtual void BeginPlay() override;

private:	
	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
		class AController* InstigatedBy, AActor* DamageCauser );
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat | Health")
	float MaxHealth = 100.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat | Health")
	float CurrentHealth;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat | Health")
	bool bIsDead = false;
	
	FOnDeathSignature OnDeathDelegate;
};
