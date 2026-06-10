#pragma once

#include "CoreMinimal.h"
#include "Interface/RMessageReceiver.h"
#include "UObject/NoExportTypes.h"
#include "RNPCSpawnManager.generated.h"

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

class URPathFinder;
class ARNPCCharacter;
UCLASS(Blueprintable)
class RESONANCE_API URNPCSpawnManager : public UObject, public IRMessageReceiver
{
	GENERATED_BODY()
	
public:
	static URNPCSpawnManager* Get(UObject* Context);
	void Initialize();
	void DeInitialize();
	
protected:
	virtual void OnMessage(ERMessageType Type, FRMessage* Message);
	
	void InitSpawnNPC();
	void SpawnNPC(const FName& RegionID);
	
protected:
	
	// 풀링과 관련된 함수
	void AcquireNPC(FName NPCID);
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
};
