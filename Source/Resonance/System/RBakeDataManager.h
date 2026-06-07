#pragma once

#include "CoreMinimal.h"
#include "Data/RCoreStructs.h"
#include "RBakeDataManager.generated.h"

struct FRPointData;
enum class ERBakeType : uint8;

UCLASS()
class RESONANCE_API URBakeDataManager : public UObject
{
	GENERATED_BODY()

public:
	static URBakeDataManager* Get(UObject* Context);
	const FRBakeDataGroup& GetBakeDataGroup() { return BakeDataGroup; }
	bool GetRandomTransformData(ERBakeType BakeType, FRPointData& OutData) const;
	bool GetTransformData(ERBakeType BakeType, TArray<FRPointData>& OutData) const;
	bool GetLocationData(ERBakeType BakeType, TArray<FVector>& OutData) const;
	
	void Initialize();
	
protected:
	
	UPROPERTY(Transient)
	FRBakeDataGroup BakeDataGroup;
	
};
