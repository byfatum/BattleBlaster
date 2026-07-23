#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Containers/Set.h"

#include "BattleBlasterGameMode.generated.h"

class ABasePawn;
class ATower;

enum class EGameResult : uint8
{
	InProgress, 
	Victory,
	Defeat
};

UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABattleBlasterGameMode();
	void RegisterPawn(ABasePawn* const Pawn);
	void ActorDied(ABasePawn* const Pawn);
	bool IsGameplayEnabled() const;
	EGameResult GetGameResult() const;
	
	DECLARE_EVENT(ABattleBlasterGameMode, FOnGameplayEnabledChanged)
	FOnGameplayEnabledChanged& OnGameplayEnabledChanged();
	
	DECLARE_EVENT(ABattleBlasterGameMode, FOnGameResultChanged)
	FOnGameResultChanged& OnGameResultChanged();
	
protected:
	virtual void BeginPlay() override;
	
private:
	bool AllTowersDead() const;
	void FinishGame(EGameResult GameResult);
	void HandleGameResult() const;
	void SetGameplayEnabled(bool NewGameplayEnabled);
	void OnCountDownTimerTimeout();
	
	TSet<TWeakObjectPtr<ATower>> AliveTowers;
	EGameResult CurrentGameResult = EGameResult::InProgress;
	
	UPROPERTY(EditDefaultsOnly, Category = "Time", meta = (ClampMin = 0.0f));
	float RestartDelay = 3.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	bool bIsGameplayEnabled = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float CountDownDelay = 3.0f;
	
	FOnGameplayEnabledChanged OnGameplayEnabledChangedSignature;
	FOnGameResultChanged OnGameResultChangedSignature;
};