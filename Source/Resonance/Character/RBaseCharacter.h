#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "RBaseCharacter.generated.h"

class URActionStateComponent;
class URBaseStatComponent;
UCLASS()
class RESONANCE_API ARBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARBaseCharacter();
	
	URBaseStatComponent* GetBaseStatComponent() const { return BaseStatComponent; }
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<URBaseStatComponent> BaseStatComponent;
};