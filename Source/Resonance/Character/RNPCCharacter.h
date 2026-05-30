#pragma once

#include "CoreMinimal.h"
#include "Character/RBaseCharacter.h"
#include "RNPCCharacter.generated.h"
class URActionData;
class UStateTreeAIComponent;

UCLASS()
class RESONANCE_API ARNPCCharacter : public ARBaseCharacter
{
	GENERATED_BODY()
	
public:
	ARNPCCharacter();
	
	virtual void BeginPlay() override;
	
};
