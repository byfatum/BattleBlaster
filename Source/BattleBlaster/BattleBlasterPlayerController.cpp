#include "BattleBlasterPlayerController.h"
#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ScreenMessage.h"

#define LOCTEXT_NAMESPACE "BattleBlasterPlayerController"

ABattleBlasterPlayerController::ABattleBlasterPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ABattleBlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<ABattleBlasterGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode.Get())
	{
		ScreenMessageObject = CreateWidget<UScreenMessage>(this, ScreenMessageClass);
		if (!ScreenMessageObject) return;
		
		ScreenMessageObject->AddToPlayerScreen();
		ScreenMessageObject->SetMessageText(LOCTEXT("GameplayStarting", "Get Ready!"));
		
		GameMode->OnGameplayEnabledChanged().AddUObject(this, &ABattleBlasterPlayerController::HandleGameplayEnabledChanged);
		GameMode->OnGameResultChanged().AddUObject(this, &ABattleBlasterPlayerController::HandleGameResultChanged);
		GameMode->OnCountDownDelayChanged().AddUObject(this, &ABattleBlasterPlayerController::HandleRemainingCountDownSeconds);
	}
}

void ABattleBlasterPlayerController::HandleGameplayEnabledChanged()
{
	FTimerHandle HideScreenMessage;
	GetWorldTimerManager().SetTimer(
		HideScreenMessage,
		this, 
		&ABattleBlasterPlayerController::ApplyGameplayEnabledChanged, 
		1.0f, 
		false
	);
}

void ABattleBlasterPlayerController::HandleGameResultChanged()
{
	ApplyGameResultChanged(GameMode->GetGameResult());
}

void ABattleBlasterPlayerController::HandleRemainingCountDownSeconds()
{
	ApplyRemainingCountDownSeconds(GameMode->GetRemainingCountDownSeconds());
}

void ABattleBlasterPlayerController::ApplyGameplayEnabledChanged()
{
	ScreenMessageObject->SetVisibility(
		GameMode->IsGameplayEnabled() && GameMode->GetGameResult() == EGameResult::InProgress ? 
		ESlateVisibility::Collapsed : ESlateVisibility::Visible
	);
}

void ABattleBlasterPlayerController::ApplyGameResultChanged(EGameResult GameResult)
{
	ScreenMessageObject->SetMessageText(
		GameResult == EGameResult::Victory ? 
		LOCTEXT("GameResultVictory", "Victory!") : 
		LOCTEXT("GameResultDefeat", "Defeat!")
	);
	
	ScreenMessageObject->SetVisibility(ESlateVisibility::Visible);
}

void ABattleBlasterPlayerController::ApplyRemainingCountDownSeconds(float RemainingCountDownSeconds)
{
	ScreenMessageObject->SetMessageText(
		RemainingCountDownSeconds <= 0 ? 
		LOCTEXT("CountDownTimerTimeoutOver", "Go!") : FText::AsNumber(RemainingCountDownSeconds)
	);
}

#undef LOCTEXT_NAMESPACE