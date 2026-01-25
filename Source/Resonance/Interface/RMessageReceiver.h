#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RMessageReceiver.generated.h"

enum class ERMessageType;
struct FRMessage;
UINTERFACE(MinimalAPI)
class URMessageReceiver : public UInterface
{
	GENERATED_BODY()
};


class RESONANCE_API IRMessageReceiver
{
	GENERATED_BODY()

public:

	virtual void OnMessage(ERMessageType Type, FRMessage* Message = nullptr) = 0;
};