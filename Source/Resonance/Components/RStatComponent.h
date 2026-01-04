#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	URStatComponent();

protected:
	
	virtual void BeginPlay() override;

		
};
