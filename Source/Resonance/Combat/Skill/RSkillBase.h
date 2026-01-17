#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RSkillBase.generated.h"

UENUM()
enum class ERSkillType : uint8
{
	Default,		// 기본
	ESkill,			// ESkill
	QSkill,			// QSkill
};

class UAnimMontage;

// 블루프린트로 만들 수 있음.
UCLASS(Abstract)
class RESONANCE_API URSkillBase : public UObject
{
	GENERATED_BODY()

public:
	URSkillBase();

	// 구조체로 Context 넘길 예정
	bool TryAttack();

protected:

	virtual void Attack() {}

protected:

	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	ERSkillType SkillType;

	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	float CoolTime;

protected:

	// 스킬 사용 시간
	UPROPERTY(Transient)
	double LastCastTime;

};
