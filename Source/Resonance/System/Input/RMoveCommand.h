#pragma once

#include "CoreMinimal.h"
#include "System/Input/Base/RInputCommand.h"
#include "RMoveCommand.generated.h"

UCLASS(Blueprintable)
class RESONANCE_API URMoveCommand : public URInputCommand
{
	GENERATED_BODY()
	
public:
	virtual void Exec() override;
};
