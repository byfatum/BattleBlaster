#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "Tank.generated.h"

UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SetupInputMappingContext();
	void MoveInput(const struct FInputActionValue& Value);
	void TurnInput(const struct FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> TurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float MoveSpeed = 500.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float TurnRate = 125.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sphere")
	float SphereRadius = 20.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sphere")
	int SphereSegments = 20;
};
