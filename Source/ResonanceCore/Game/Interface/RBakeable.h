#pragma once

#include "CoreMinimal.h"
#include "Data/RCoreEnums.h"
#include "UObject/Interface.h"
#include "RBakeable.generated.h"

enum class ERBakeType : uint8;
struct FRTransformData;

UINTERFACE()
class RESONANCECORE_API URBakeable : public UInterface
{
	GENERATED_BODY()
};

class RESONANCECORE_API IRBakeable
{
	GENERATED_BODY()

public:
	
	virtual FRTransformData GetSerializedData() const = 0;
	virtual ERBakeType GetBakeType() const = 0;
};
