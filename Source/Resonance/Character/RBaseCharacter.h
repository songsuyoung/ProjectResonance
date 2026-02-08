#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "RBaseCharacter.generated.h"

class URStateContext;

UCLASS()
class RESONANCE_API ARBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARBaseCharacter();

	void PushStateGameTag(const FGameplayTag& StateTag);
	void PopStateGameTag(const FGameplayTag& StateTag);

protected:

	// 캐릭터의 전체 상태를 관리할 예정
	UPROPERTY(Transient)
	FGameplayTagContainer CurrentStateTags;

	UPROPERTY(Transient)
	TObjectPtr<URStateContext> StateContext;
	
};
