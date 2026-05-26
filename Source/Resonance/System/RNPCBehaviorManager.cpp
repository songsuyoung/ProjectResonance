#include "System/RNPCBehaviorManager.h"

FName URNPCBehaviorManager::GetRandomAction()
{
	return FName();
}

UAnimMontage* URNPCBehaviorManager::FindAction(const FName& ActionName)
{
	return nullptr;
}

bool URNPCBehaviorManager::CheckAction(const FName& VerbActionName, const FName& NounActionName)
{
	return false;
}
