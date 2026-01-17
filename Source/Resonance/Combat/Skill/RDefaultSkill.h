#pragma once

#include "CoreMinimal.h"
#include "Combat/Skill/RSkillBase.h"
#include "RDefaultSkill.generated.h"

UCLASS(Blueprintable)
class RESONANCE_API URDefaultSkill : public URSkillBase
{
	GENERATED_BODY()

public:

	virtual void Attack() override;

protected:


};
