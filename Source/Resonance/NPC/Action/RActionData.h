#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RActionData.generated.h"

UENUM(BlueprintType)
enum class ERDurationType : uint8
{
	None,			// DurationType을 가지지않음
	Infinite,		// 무한대
	AnimationBased,	// 몽타주 길이 따라감
	TimeBased,		// 시간 설정 필요
	EventBased,		// 이벤트에 의해 처리
};

UCLASS(Blueprintable)
class RESONANCE_API URActionData : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "NPC | Action")
	FName ActionName;
	
	UPROPERTY(EditAnywhere, Category = "NPC | Action")
	FTransform ActionTransform;
	
	UPROPERTY(EditAnywhere, Category = "NPC | Action")
	TArray<FName> ComposableActions;
	
	UPROPERTY(EditAnywhere, Category = "NPC | Action")
	TArray<FName> IncomposableActions;
	
	UPROPERTY(EditAnywhere, Category = "NPC | Action")
	ERDurationType DurationType;
	
	UPROPERTY(EditAnywhere, Category = "NPC | Action", meta = (EditCondition = "DurationType == ERDurationType::TimeBased"))
	float Duration;
};
