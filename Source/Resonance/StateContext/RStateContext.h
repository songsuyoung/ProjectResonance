#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "RStateContext.generated.h"

/*
UObject 는 UWorld, Time, Replicate 불가능

고려해야하는 점 
- 교체 방식이 아님 A -> B (A를 취소해야하는가? 문제점이 있음.)
- 이 문제점을 해결하기 위해서는 
	- 취소 태그
	- 등록가능한가 확인 태그
	- 등록할 때 같이 있어선 안되는 태그
	- 등록될 때 부착되는 태그
*/
UENUM(Blueprintable)
enum class ERContextInstancePolicty : uint8
{
	Single, // 중첩 실행 불가능
	Multiple, // 중첩 실행 가능
};

UENUM(Blueprintable)
enum class ERContextExecutionType : uint8
{
	Instance,	// 단일 종료
	Sustained,  // 명시적 종료를 호출 할 때까지 지속
};
USTRUCT(BlueprintType)
struct FRTransitionCondition
{
	GENERATED_BODY()

public:
	// 취소 태그
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer CancelTags;	
	
	// 발동 조건`
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer RequiredTags;

	// 금지 조건
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer BlockedTags;
	
	// 현재 Condition이 실행될 때 부착될 Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer CurrentTags;
};

class URActionStateComponent;

// 블루프린트화 시킬 수 있다.
UCLASS(Blueprintable)
class RESONANCE_API URStateContext : public UObject
{
	GENERATED_BODY()
	
public:

	// 생성 시 호출
	virtual void Initialize(URActionStateComponent* InActionStateComponent);
	
	// Condition 여부를 확인 예정
	bool CanTransition(const FGameplayTagContainer& InContainer);
	void ExecuteContext(FGameplayTagContainer& InContainer);
	virtual void OnTick(float DeltaTime) { }
	virtual void Execute() { }
	
	virtual bool OnEnter() { return true; }
	virtual void OnExit();
	
public:
	bool IsCancel(const FGameplayTagContainer& CancelContainer);
	int32 GetPriority() { return Priority; }
	bool CanTick() { return bCanTick; }
	FGameplayTagContainer GetCancelTags() { return Condition.CancelTags; }

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Condition")
	FRTransitionCondition Condition;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Condition")
	ERContextExecutionType ExecutionType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Condition")
	ERContextInstancePolicty InstancePolicy;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Condition")
	int32 Priority;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Condition")
	uint8 bCanTick : 1;
	
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URActionStateComponent> ActionStateComponent;
};
