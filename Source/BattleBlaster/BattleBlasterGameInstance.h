#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "BattleBlasterGameInstance.generated.h"

class UWorld;

UCLASS()
class BATTLEBLASTER_API UBattleBlasterGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()
	
public:
	void SetCurrentLevelIndex(const UWorld* const CurrentWorld);
	void LoadNextLevel();
	void RestartCurrentLevel();
	
private:
	void ChangeLevel() const;
	void RestartGame() const;
	
	UPROPERTY(VisibleAnywhere, Category = "Levels | LevelIndices")
	int32 CurrentLevelIndex;
	
	UPROPERTY(EditDefaultsOnly, Category = "Levels | Maps")
	TArray<TSoftObjectPtr<UWorld>> CampaignMaps;
};
