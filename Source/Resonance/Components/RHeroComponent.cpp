#include "Components/RHeroComponent.h"

// UE 5.
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

// Newly Created File 

URHeroComponent::URHeroComponent()
	: Super()
{
}

void URHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetOwner());

	if (PlayerController.IsValid())
	{
		OwnerPawn = Cast<ACharacter>(PlayerController->GetPawn());
	}
}

void URHeroComponent::SetupInputComponent()
{
	// 상위 클래스에서 InputComponent == nullptr 일 경우에 새로 생성함.
	if (false == PlayerController.IsValid())
	{
		//다시 가져온다.
		PlayerController = Cast<APlayerController>(GetOwner());
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (PlayerController.IsValid())
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

		if (IsValid(EnhancedInputComponent))
		{
			EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
			EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
			EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ThisClass::StartJump);
		}
	}
}


void URHeroComponent::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (PlayerController.IsValid())
	{
		const FRotator Rotation = PlayerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (OwnerPawn.IsValid())
		{
			OwnerPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
			OwnerPawn->AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void URHeroComponent::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (PlayerController.IsValid())
	{
		if (OwnerPawn.IsValid())
		{
			OwnerPawn->AddControllerYawInput(LookAxisVector.X);
			OwnerPawn->AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void URHeroComponent::StartJump()
{
}
