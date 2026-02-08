#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "RStateContext.generated.h"

/*
UObject 는 UWorld, Time, Replicate 물가능
*/
UCLASS()
class RESONANCE_API URStateContext : public UObject
{
	GENERATED_BODY()
	
public:

	void PushTag(const FGameplayTag& StateTag);
	void PopTag(const FGameplayTag& StateTag);
	bool ContainTag(const FGameplayTag& StateTag);

protected:

	// 캐릭터의 전체 상태를 관리할 예정
	UPROPERTY(Transient)
	FGameplayTagContainer CurrentStateTags;
};
