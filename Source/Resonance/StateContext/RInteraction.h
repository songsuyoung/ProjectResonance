#pragma once

#include "CoreMinimal.h"
#include "RStateContext.h"
#include "RInteraction.generated.h"

class ACharacter;

// 블루프린트화 시킬 수 있다.
UCLASS(Blueprintable)
class RESONANCE_API URInteraction : public URStateContext
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(AActor* InOwner) override;
	
	virtual void Execute() override;
	
protected:
	
	// 범위 (즉, 탐색 거리를 의미)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Range")
	float SearchRange;
	
	// 0~360도 사이의 각도를 이룬다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Range")
	float SearchFOV;
	
protected:
	
	UPROPERTY(Transient)
	float SearchRadianRange;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	
};
