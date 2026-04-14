#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RInteractable.generated.h"

UINTERFACE(MinimalAPI)
class URInteractable : public UInterface
{
	GENERATED_BODY()
};

class RESONANCE_API IRInteractable
{
	GENERATED_BODY()

public:
	
	virtual void Interact(AActor* Other) = 0;
};
