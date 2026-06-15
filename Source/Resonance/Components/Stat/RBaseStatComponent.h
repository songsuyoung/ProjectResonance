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
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentStatValue(ERStatType StatType);
	
	UFUNCTION(BlueprintCallable)
	float GetMaxStatValue(ERStatType StatType);
	
	UFUNCTION(BlueprintCallable)
	void UpdateStat(ERStatType StatType, float NewStatValue);
	
protected:
	void SetupStat(const TArray<FRStatInfo>& InStatInfos);
	virtual void BeginPlay() override;

protected:

	UPROPERTY(Transient)
	TArray<FRStatInfo> StatInfos;
	
	UPROPERTY(Transient)
	FName OwnerID;
};
