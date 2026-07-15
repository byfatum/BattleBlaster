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
	
private:
	bool AllTowersDead() const;
	void FinishGame(EGameResult GameResult);
	void RestartCurrentLevel() const;
	
	TSet<TWeakObjectPtr<ATower>> AliveTowers;
	EGameResult CurrentGameResult = EGameResult::InProgress;
	
	UPROPERTY(EditDefaultsOnly, Category = "Time", meta = (ClampMin = 0.0f));
	float RestartDelay = 3.0f;
};
