#pragma once

#include "CoreMinimal.h"
#include "Interface/RMessageReceiver.h"
#include "UObject/NoExportTypes.h"
#include "RNPCSpawnManager.generated.h"

class URPathFinder;
class ARNPCCharacter;

USTRUCT(BlueprintType)
struct FRNPCPooling
{
	GENERATED_BODY()
public:
	
	UPROPERTY(Transient)
	TObjectPtr<ARNPCCharacter> NPC;
	
	UPROPERTY(Transient)
	float PooledTime;		// 풀링에 들어온 시간 (타임스탬프)
	
	UPROPERTY(Transient)
	FTimerHandle SpawnTimerHandle;
};

UCLASS(Blueprintable)
class RESONANCE_API URNPCSpawnManager : public UObject, public IRMessageReceiver
{
	GENERATED_BODY()
	
public:
	static URNPCSpawnManager* Get(UObject* Context);
	void Initialize();
	void DeInitialize();
	ARNPCCharacter* GetObserveTarget();

	// 풀링과 관련된 함수
	void AcquireNPC(FName NPCID);
protected:
	virtual void OnMessage(ERMessageType Type, FRMessage* Message);
		
	void InitSpawnNPC();
	void SpawnNPC(const FName& NPCID, const FName& RegionID);
	
protected:
	void ReleaseNPC(const FRMessage* Msg);
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<ARNPCCharacter> NPCCharacterClass;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URPathFinder> PathFinder;
	
	UPROPERTY(Transient)
	TMap<FName, FRNPCPooling> NPCPooling;
	
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<ARNPCCharacter>> ActiveNPC;
	
	UPROPERTY(Transient)
	TArray<FName> NPCIDs;
	
	UPROPERTY(Transient)
	int32 ObserverIndex;
};
