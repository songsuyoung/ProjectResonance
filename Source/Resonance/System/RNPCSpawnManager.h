#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RNPCSpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FRPooling
{
	GENERATED_BODY()
public:
	
};

USTRUCT(BlueprintType)
struct FRZoneSpawnedNPCs
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TSet<int32> SpawnedNPCIds;
};

class URPathFinder;
class ARNPCCharacter;
UCLASS(Blueprintable)
class RESONANCE_API URNPCSpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	static URNPCSpawnManager* Get(UObject* Context);
	
	void SpawnNPC(const FName& RegionID, const FTransform& SpawnTransform);
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<ARNPCCharacter> NPCCharacterClass;
	
protected:
	
	UPROPERTY(Transient)
	TArray<int32> SpawnPointIndex;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URPathFinder> PathFinder;
	
	UPROPERTY(Transient)
	TArray<FRPooling> NPCPooling;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<ARNPCCharacter>> ActiveNPC;
	
	// 존에 따라서 스폰된 NPC 관리 => 중복 스폰을 막기 위해서
	// NPC마다 ID을 부착해서, 중복으로 태어났는지 아닌지를 확인해야한다.
	UPROPERTY(Transient)
	TMap<int32, FRZoneSpawnedNPCs> NPCIDs; 
};
