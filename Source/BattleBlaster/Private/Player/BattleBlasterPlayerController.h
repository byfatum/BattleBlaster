#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattleBlasterPlayerController.generated.h"

class ABattleBlasterGameMode;
class UScreenMessage;
enum class EGameResult : uint8;
class UCameraShakeBase;

UCLASS()
class BATTLEBLASTER_API ABattleBlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABattleBlasterPlayerController();
	
	void StartHitCameraShake() const;
	void StartDeathCameraShake() const;
	
protected:
	virtual void BeginPlay() override;
	
	void HandleGameplayEnabledChanged();
	void HandleGameResultChanged();
	void HandleRemainingCountDownSeconds();
	
	void ApplyGameplayEnabledChanged();
	void ApplyGameResultChanged(EGameResult GameResult);
	void ApplyRemainingCountDownSeconds(float RemainingCountDownSeconds);
	
private:
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<ABattleBlasterGameMode> GameMode;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UScreenMessage> ScreenMessageClass;
	
	UPROPERTY(Transient)
	TObjectPtr<UScreenMessage> ScreenMessageObject;
	
	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> HitCameraShake;
	
	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> DeathCameraShake;
};