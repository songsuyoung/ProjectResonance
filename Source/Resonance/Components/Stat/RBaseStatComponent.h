#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ResonanceStructs.h"
#include "RBaseStatComponent.generated.h"

enum class ERStatType : uint8;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStatChanged, ERStatType, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URBaseStatComponent();
	
	FOnStatChanged OnStatChanged;
	
	void SetViewTarget(bool bView);
	
	float GetCurrentStatValue(ERStatType StatType);
	
	float GetMaxStatValue(ERStatType StatType);
	
	void UpdateStat(ERStatType StatType, float NewStatValue);
	
	void RecoverStatOnSpawn(ERStatType StatType, float RecoveryRatio);
	
	void GetRegionPreferences(TMap<FName, float>& PreferredRegions, TMap<FName, float>& DislikedRegions);
protected:
	void SetRegionPreferences(const TMap<FName, float>& InPreferredRegions, const TMap<FName, float>& InDislikedRegions);
	void SetupStat(const TArray<FRStatInfo>& InStatInfos) { StatInfos = InStatInfos; }
	virtual void BeginPlay() override;

protected:

	UPROPERTY(Transient)
	TArray<FRStatInfo> StatInfos;
	
	UPROPERTY(Transient)
	TMap<FName, float> PreferredRegions;
	
	UPROPERTY(Transient)
	TMap<FName, float> DislikedRegions;
	
	UPROPERTY(Transient)
	FName OwnerID;
	
	UPROPERTY(Transient)
	uint8 bViewTarget : 1;
};
