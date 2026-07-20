#include "BattleBlasterGameMode.h"
#include "BattleBlasterPlayerController.h"
#include "BasePawn.h"
#include "BattleBlasterGameInstance.h"
#include "Tank.h"
#include "Tower.h"
#include "Engine/TimerHandle.h"

ABattleBlasterGameMode::ABattleBlasterGameMode()
{
	PlayerControllerClass = ABattleBlasterPlayerController::StaticClass();
}

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
			FinishGame(EGameResult::Victory);
		}
	}
	else if (ATank* const Tank = Cast<ATank>(Pawn))
	{
		FinishGame(EGameResult::Defeat);
	}
}

ABattleBlasterGameMode::FOnGameplayEnabledChanged& ABattleBlasterGameMode::OnGameplayEnabledChanged() 
{
	return OnGameplayEnabledChangedSignature;
}

bool ABattleBlasterGameMode::IsGameplayEnabled() const
{
	return bIsGameplayEnabled;
}

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (UBattleBlasterGameInstance* const GameInstance = Cast<UBattleBlasterGameInstance>(GetGameInstance()))
	{
		GameInstance->SetCurrentLevelIndex(GetWorld());
	}
	
	FTimerHandle GameplayStart;
	GetWorldTimerManager().SetTimer(
		GameplayStart, 
		this, 
		&ABattleBlasterGameMode::OnCountDownTimerTimeout,
		CountDownDelay,
		false
	);
}

bool ABattleBlasterGameMode::AllTowersDead() const
{
	return AliveTowers.Num() == 0;
}

void ABattleBlasterGameMode::FinishGame(EGameResult GameResult)
{
	if (CurrentGameResult != EGameResult::InProgress) return;
	CurrentGameResult = GameResult;
	
	if (CurrentGameResult == EGameResult::Victory)
	{
		UE_LOG(LogTemp, Display, TEXT("Victory!"));
	}
	else if (CurrentGameResult == EGameResult::Defeat)
	{
		UE_LOG(LogTemp, Display, TEXT("Defeat!"));
	}
	
	FTimerHandle GameOverHandle;
	GetWorldTimerManager().SetTimer(
		GameOverHandle,
		this,
		&ABattleBlasterGameMode::HandleGameResult,
		RestartDelay,
		false
	);
}

void ABattleBlasterGameMode::HandleGameResult() const
{
	if (UBattleBlasterGameInstance* const GameInstance = Cast<UBattleBlasterGameInstance>(GetGameInstance()))
	{
		if (CurrentGameResult == EGameResult::Victory)
		{
			GameInstance->LoadNextLevel();
		}
		else if (CurrentGameResult == EGameResult::Defeat)
		{
			GameInstance->RestartCurrentLevel();
		}
	}
}

void ABattleBlasterGameMode::SetGameplayEnabled(bool NewGameplayEnabled)
{
	bIsGameplayEnabled = NewGameplayEnabled;
	OnGameplayEnabledChangedSignature.Broadcast();
}

void ABattleBlasterGameMode::OnCountDownTimerTimeout()
{
	SetGameplayEnabled(true);
}