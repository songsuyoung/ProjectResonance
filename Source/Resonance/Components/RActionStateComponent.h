#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RActionStateComponent.generated.h"

class URStateContext;
enum class ERActionContext : uint8;

DECLARE_MULTICAST_DELEGATE(FOnGameplayTagChanged);

struct FRDelegateInfo
{
	FOnGameplayTagChanged	OnNewOrRemove;
};

USTRUCT(BlueprintType)
struct FRStateContextClassContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input | Action")
	TArray<TSubclassOf<URStateContext>> ActionContextClasses;
};

USTRUCT(BlueprintType)
struct FRStateContextContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TArray<TObjectPtr<URStateContext>> ActionContexts;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URActionStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URActionStateComponent();
	
	void PushState(const ERActionContext& InputState);
	FGameplayTagContainer& GetActiveGameplayTagContainer() { return Container; }
	URStateContext* FindState(const ERActionContext& InputState);
	FOnGameplayTagChanged& RegisterGameplayTagEvent(const ERActionContext& InputState);

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	
	// 미리 StateContext를 만들어야 한다.
	// TSubclassOf (로딩될 때 모두 클래스 로딩을 선언=>비동기로딩 ㄴㄴ)
	// 이때 사용할 Enum값 정의, (Input에 의해 전달될 예정)
	UPROPERTY(EditDefaultsOnly, Category = "Input | Action")
	TMap<ERActionContext, FRStateContextClassContainer> ActionContextClass; 
protected:
	
	UPROPERTY(Transient)
	TMap<ERActionContext, FRStateContextContainer> ActionContexts;
	
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<URStateContext>> CurrentActionContexts;
	
	// 실행 중인 Container
	UPROPERTY(Transient, BlueprintReadOnly)
	FGameplayTagContainer Container;
	
	TMap<ERActionContext, FRDelegateInfo> EventMap;
};
