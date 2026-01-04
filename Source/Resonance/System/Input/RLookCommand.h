#pragma once

#include "CoreMinimal.h"
#include "System/Input/Base/RInputCommand.h"
#include "RLookCommand.generated.h"

UCLASS(Blueprintable)
class RESONANCE_API URLookCommand : public URInputCommand
{
	GENERATED_BODY()

public:

	virtual void Exec() override;
	
};
