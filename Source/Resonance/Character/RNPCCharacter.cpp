#include "Character/RNPCCharacter.h"

#include "NPC/Action/RActionData.h"

void ARNPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ExecuteAction();
}

void ARNPCCharacter::ExecuteAction()
{
	if (ActionData.IsEmpty())
	{
		for (int32 Index = 0; Index < ActionDataClass.Num(); Index++)
		{
			ActionData.Add(NewObject<URActionData>(this, ActionDataClass[Index]));
		}
	}
	
	// 몽타주 실행
	GetActionName();
}

FName ARNPCCharacter::GetActionName()
{
	FString ActionName = TEXT("");
	
	for (int32 Index = 0; Index < ActionData.Num() - 1; Index++)
	{
		ActionName = ActionData[Index]->ActionName.ToString() + "_";
	}
	
	ActionName += ActionData.Last()->ActionName.ToString();
	
	return FName(ActionName);
}
