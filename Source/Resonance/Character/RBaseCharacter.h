#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "RBaseCharacter.generated.h"

class URActionStateComponent;
UCLASS()
class RESONANCE_API ARBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARBaseCharacter();

	
protected:

	// 캐릭터의 전체 상태를 관리할 예정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<URActionStateComponent> ActionStateComponent;
	
};