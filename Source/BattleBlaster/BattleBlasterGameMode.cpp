#include "BattleBlasterGameMode.h"
#include "BasePawn.h"
#include "Tank.h"
#include "Tower.h"

void ABattleBlasterGameMode::RegisterPawn(ABasePawn* const Pawn)
{
	if (ATower* const Tower = Cast<ATower>(Pawn))
	{
		AliveTowers.Add(Tower);
	}
}

void ABattleBlasterGameMode::ActorDied(ABasePawn* const Pawn)
{
	if (ATower* const Tower = Cast<ATower>(Pawn))
	{
		AliveTowers.Remove(Tower);
		
		if (AllTowersDead())
		{
			UE_LOG(LogTemp, Display, TEXT("You Victory! All Towers are dead"));
		}
	}
	else if (ATank* const Tank = Cast<ATank>(Pawn))
	{
		UE_LOG(LogTemp, Display, TEXT("You Lose! Try Again"));
	}
}

bool ABattleBlasterGameMode::AllTowersDead() const
{
	return AliveTowers.Num() == 0;
}
