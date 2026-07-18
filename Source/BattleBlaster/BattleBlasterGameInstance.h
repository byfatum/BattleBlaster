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
	void RestartCurrentLevel() const;
	
protected:
	virtual void Init();
	
private:
	void ChangeLevel();
	void RestartCampaign() const;
	void RestartCurrentLevelByPackageName() const;
	bool ValidateCampaignMaps() const;
	
	UPROPERTY(VisibleAnywhere, Category = "Campaign")
	int32 CurrentLevelIndex = INDEX_NONE;
	
	UPROPERTY(EditDefaultsOnly, Category = "Campaign")
	TArray<TSoftObjectPtr<UWorld>> CampaignMaps;
	
	UPROPERTY(VisibleAnywhere, Category = "Campaign")
	bool bIsCampaignValid;
};
