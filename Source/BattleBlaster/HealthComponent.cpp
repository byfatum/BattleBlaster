#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UHealthComponent::FOnDeathSignature& UHealthComponent::OnDeath()
{
	return OnDeathDelegate;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!bIsDead)
	{
		CurrentHealth = MaxHealth;
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);
	}
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bIsDead && Damage > 0.0)
	{
		CurrentHealth -= Damage;
		
		if (CurrentHealth <= 0.0f)
		{
			bIsDead = true;
			OnDeathDelegate.Broadcast();
		}
	}
}

