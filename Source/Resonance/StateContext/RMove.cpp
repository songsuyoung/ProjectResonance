#include "StateContext/RMove.h"

#include "EnhancedPlayerInput.h"
#include "GameFramework/Character.h"

void URMove::Initialize(URActionStateComponent* InActionStateComponent)
{
	Super::Initialize(InActionStateComponent);
		
	CharacterRef = Cast<ACharacter>(GetOuter());
	
	if (CharacterRef.IsValid())
	{
		PlayerControllerRef = Cast<APlayerController>(CharacterRef->GetController());
	
		EnhancedComponentRef = Cast<UEnhancedPlayerInput>(PlayerControllerRef->PlayerInput);
	}
}

void URMove::Execute()
{
	if (false == EnhancedComponentRef.IsValid())
	{
		return;
	}
	
	if (false == PlayerControllerRef.IsValid())
	{
		return;
	}
	
	if (false == CharacterRef.IsValid())
	{
		return;
	}
	
	FInputActionValue ActionValue = EnhancedComponentRef->GetActionValue(MoveInputAction);

	FVector2D MovementVector = ActionValue.Get<FVector2D>();

	const FRotator Rotation = PlayerControllerRef->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (CharacterRef.IsValid())
	{
		CharacterRef->AddMovementInput(ForwardDirection, MovementVector.Y);
		CharacterRef->AddMovementInput(RightDirection, MovementVector.X);
	}
	
}
