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

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<class UStaticMeshComponent> BaseComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<class UStaticMeshComponent> TurretComponent;
};
