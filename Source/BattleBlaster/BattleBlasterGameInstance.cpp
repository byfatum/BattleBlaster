#include "BattleBlasterGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Set.h"

void UBattleBlasterGameInstance::SetCurrentLevelIndex(const UWorld* const CurrentWorld)
{
	CurrentLevelIndex = INDEX_NONE;
	if (!CurrentWorld) return;
	
	if (bIsCampaignValid)
	{
		const FString CleanCurrentWorldPackageName = UWorld::RemovePIEPrefix(CurrentWorld->GetPackage()->GetName());

		for (int32 Index = 0; Index < CampaignMaps.Num(); ++Index)
		{
			if (CleanCurrentWorldPackageName == CampaignMaps[Index].GetLongPackageName())
			{
				CurrentLevelIndex = Index;
				break;
			}
		}
	}
}

void UBattleBlasterGameInstance::LoadNextLevel()
{
	/*
	 bIsCampaignValid is: 
	 1) Campaign empty; 2) Campaign[Index] can be IsNull(); 
	 3) Duplicate map package.
	*/
	if (!bIsCampaignValid)
	{
		UE_LOG(
			LogTemp, 
			Error, 
			TEXT("CampaignMaps Configuration is not valid")
		);
			
		RestartCurrentLevelByPackageName();
	}
	else if (!CampaignMaps.IsValidIndex(CurrentLevelIndex))
	{
		UE_LOG(
			LogTemp,
			Display,
			TEXT("Campaign Configuration is valid, but current map is not registered in CampaignMaps")
		);
		
		RestartCurrentLevelByPackageName();
	}
	else if (CampaignMaps.IsValidIndex(CurrentLevelIndex))
	{
		ChangeLevel();
	}
}

void UBattleBlasterGameInstance::RestartCurrentLevel() const
{
	RestartCurrentLevelByPackageName();
}

void UBattleBlasterGameInstance::Init()
{
	Super::Init();
	
	bIsCampaignValid = ValidateCampaignMaps();
}

/*
1) Campaign is not empty; 2) Campaign[Index] cant be IsNull(); 
3) Campaign dont have duplicate map package; 4) Index is valid.
*/
void UBattleBlasterGameInstance::ChangeLevel() 
{
	if (CurrentLevelIndex >= 0 && CurrentLevelIndex < CampaignMaps.Num() - 1)
	{
		const int32 NextLevelIndex = CurrentLevelIndex + 1;
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, CampaignMaps[NextLevelIndex]);
	} 
	else if (CurrentLevelIndex == CampaignMaps.Num() - 1)
	{
		RestartCampaign();
	}
}

void UBattleBlasterGameInstance::RestartCampaign() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, CampaignMaps[0]);
}

void UBattleBlasterGameInstance::RestartCurrentLevelByPackageName() const
{
	if (const UWorld* const CurrentWorld = GetWorld())
	{
		const FString CleanCurrentWorldPackageName = UWorld::RemovePIEPrefix(CurrentWorld->GetPackage()->GetName());
		UGameplayStatics::OpenLevel(this, FName(*CleanCurrentWorldPackageName));
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentWorld is nullptr"));
	}
}

bool UBattleBlasterGameInstance::ValidateCampaignMaps() const
{
	if (CampaignMaps.IsEmpty())
	{
		UE_LOG(
			LogTemp, 
			Error, 
			TEXT("CampaignMaps Configuration is empty")
		);
		
		return false;
	}
	
	TSet<FString> SeenMaps;
	for (int32 Index = 0; Index < CampaignMaps.Num(); ++Index)
	{
		if (CampaignMaps[Index].IsNull())
		{
			UE_LOG(
				LogTemp, 
				Error, 
				TEXT("CampaignMaps Configuration IsNull in the %dth index"), 
				Index
			);
			
			return false;
		}
		
		const FString CampaignPackageName = CampaignMaps[Index].GetLongPackageName();
	
		if (SeenMaps.Contains(CampaignPackageName))
		{
			UE_LOG(
				LogTemp, 
				Error, 
				TEXT("Duplicate map package: %s | Duplicate map index: %d"), 
				*CampaignPackageName, 
				Index
			);
		
			return false;
		} 

		SeenMaps.Add(CampaignPackageName);
	}
	return true;
}