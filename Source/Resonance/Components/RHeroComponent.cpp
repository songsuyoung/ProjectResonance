#include "Components/RHeroComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "System/Input/Base/RInputCommand.h"

URHeroComponent::URHeroComponent()
	: Super()
{
}

void URHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	// Add Input Mapping Context
	if (IsValid(PlayerController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void URHeroComponent::SetupInputComponent()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (IsValid(PlayerController))
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

		if (IsValid(EnhancedInputComponent))
		{
			for (FInputCommandMapping& InputCommandMapping : InputCommandMappings)
			{
				URInputCommand* InputCommand = InputCommandMapping.InputCommand.GetDefaultObject();

				if (IsValid(InputCommand))
				{
					InputCommand->Initialize(PlayerController->GetPawn());
					EnhancedInputComponent->BindAction(InputCommandMapping.InputAction, InputCommand->GetTriggerEvent(), InputCommand, &URInputCommand::Exec);
				}
			}
		}
	}
}
