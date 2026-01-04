#pragma once

#include "CoreMinimal.h"
#include "System/Input/Base/RInputCommand.h"
#include "RJumpCommand.generated.h"


UCLASS(Blueprintable)
class RESONANCE_API URJumpCommand : public URInputCommand
{
	GENERATED_BODY()
	
public:
	virtual void Exec() override;
};
