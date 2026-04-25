#include "StateContext/RRun.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URRun::URRun()
	: Super()
{
	ExecutionType = ERContextExecutionType::Sustained;
}

void URRun::Initialize(AActor* InOwner)
{
	Super::Initialize(InOwner);
	
	CharacterRef = Cast<ACharacter>(InOwner);
		
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
