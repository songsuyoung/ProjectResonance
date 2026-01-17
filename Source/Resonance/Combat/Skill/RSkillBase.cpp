#include "Combat/Skill/RSkillBase.h"


URSkillBase::URSkillBase()
{

}

bool URSkillBase::TryAttack()
{
	UWorld* World = GetWorld();

	check(World);

	const double& Now = World->GetTimeSeconds(); //현재 시간초 가져온다.
	bool bCanUse = (Now - LastCastTime) > CoolTime;

	if (bCanUse)
	{
		Attack();
	}

	return bCanUse;
}
