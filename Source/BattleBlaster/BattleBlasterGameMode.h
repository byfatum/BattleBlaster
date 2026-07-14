#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Containers/Set.h"

#include "BattleBlasterGameMode.generated.h"

class ABasePawn;
class ATower;

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
	
	TSet<TWeakObjectPtr<ATower>> AliveTowers;
};
