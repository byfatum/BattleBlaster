#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TurretAimingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEBLASTER_API UTurretAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTurretAimingComponent();
	void SetTurretComponent(USceneComponent* InTurretСomponent);
	void AimAt(const FVector& TargetLocation, float DeltaTime);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class USceneComponent> ControlledTurretComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "TurretTurnSpeed")
	float InterpTurretSpeed = 180.0f;
};
