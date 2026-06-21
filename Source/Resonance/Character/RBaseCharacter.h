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
	FName GetID() { return ID; }
	void SetID(FName InID) { ID = InID; }
	URBaseStatComponent* GetBaseStatComponent() const { return BaseStatComponent; }
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<URBaseStatComponent> BaseStatComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "ID")
	FName ID;
};