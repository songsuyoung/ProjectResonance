#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RAttackAction.generated.h"

// 블루 프린트로 파생 가능
UCLASS(Blueprintable)
class RESONANCE_API URAttackAction : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	//TODO : 데이터화 시킬 예정

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> AnimMontage;

	// 어떤 데이터 타입인지 정의해야한다.
	// 예를 들어 '스킬'이라고 생각하면 편하다.
};
