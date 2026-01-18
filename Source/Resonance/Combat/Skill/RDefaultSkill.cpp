#include "Combat/Skill/RDefaultSkill.h"

void URDefaultSkill::Attack()
{
	Super::Attack();
	UE_LOG(LogTemp, Log, TEXT("[Skill] Attack"));
}
