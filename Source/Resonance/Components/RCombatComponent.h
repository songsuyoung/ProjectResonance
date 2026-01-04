#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URCombatComponent();

protected:
	virtual void BeginPlay() override;
		
};
