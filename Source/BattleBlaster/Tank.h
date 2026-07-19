#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

class UCameraComponent;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
class USpringArmComponent;

UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void ApplyGameplayEnabledState(bool NewGameplayEnabledState) override;

private:
	void SetupInputMappingContext();
	void MoveInput(const struct FInputActionValue& Value);
	void TurnInput(const struct FInputActionValue& Value);
	void HandleTankDeath();
	virtual void HandleDeath() override;
	void SetPlayerControllerActive(bool bActive);

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> TurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveSpeed = 500.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TurnRate = 125.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Debug | Cursor")
	bool bShowDebugSphereUnderCursor = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Debug | Cursor")
	float SphereRadius = 20.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Debug | Cursor")
	int SphereSegments = 20;
};
