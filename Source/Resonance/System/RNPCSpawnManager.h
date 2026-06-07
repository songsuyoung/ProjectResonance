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
class URPathFinder;
class ARNPCCharacter;
UCLASS(Blueprintable)
class RESONANCE_API URNPCSpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	static URNPCSpawnManager* Get(UObject* Context);
	
	void Initialize();
	void SpawnNPC();
	
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
	TArray<TWeakObjectPtr<ARNPCCharacter>> ActiveNPC;
};
