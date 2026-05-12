#include "StateContext/RRun.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URRun::URRun()
	: Super()
{
	ExecutionType = ERContextExecutionType::Sustained;
}

void URRun::Initialize(URActionStateComponent* InActionStateComponent)
{
	Super::Initialize(InActionStateComponent);
	
	CharacterRef = Cast<ACharacter>(GetOuter());
		
	if (CharacterRef.IsValid())
	{
		CMCRef = CharacterRef->GetCharacterMovement();
		
		if (CMCRef.IsValid())
		{
			OriginalSpeed = CMCRef->MaxWalkSpeed;
		}
	}
}

void URRun::Execute()
{
	Super::Execute();
	
	if (false == CMCRef.IsValid())
	{
		return;
	}
	
	CMCRef->MaxWalkSpeed = MaxSpeed;
}

void URRun::OnExit()
{
	Super::OnExit();
	
	if (false == CMCRef.IsValid())
	{
		return;
	}
	
	CMCRef->MaxWalkSpeed = OriginalSpeed;
}
