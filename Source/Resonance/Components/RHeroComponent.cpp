#include "Components/RHeroComponent.h"

// UE 5.
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

// Newly Created File 
#include "Character/ResonanceCharacter.h"
#include "Components/RCombatComponent.h"
#include "Data/ResonanceEnums.h"
#include "Data/RCharacterDataTable.h"
#include "Game/RPlayerController.h"

URHeroComponent::URHeroComponent()
	: Super()
{
	// 초기화
	InputHoldTime.Add({ ERInputContext::Attack, 0 });
	InputHoldTime.Add({ ERInputContext::SkillE, 0 });
	InputHoldTime.Add({ ERInputContext::SkillQ, 0 });
}

void URHeroComponent::SetupInputComponent()
{
	// 없으면 캐스팅 시도를 해본다.
	PlayerController = Cast<ARPlayerController>(GetOwner());

	if (PlayerController.IsValid())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

		if (IsValid(EnhancedInputComponent))
		{
			EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
			EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
			EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ThisClass::StartJump);
			EnhancedInputComponent->BindAction(InteractionInputAction, ETriggerEvent::Completed, this, &ThisClass::Interact);

			EnhancedInputComponent->BindAction(DefaultInputAction, ETriggerEvent::Started, this, &ThisClass::OnAttackInputPressed);
			EnhancedInputComponent->BindAction(DefaultInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnAttackInputTriggered);
			EnhancedInputComponent->BindAction(DefaultInputAction, ETriggerEvent::Completed, this, &ThisClass::OnAttackInputReleased);
			
			EnhancedInputComponent->BindAction(EInputAction, ETriggerEvent::Started, this, &ThisClass::OnSkillInputPressed, ERInputContext::SkillE);
			EnhancedInputComponent->BindAction(EInputAction, ETriggerEvent::Completed, this, &ThisClass::OnSkillInputReleased, ERInputContext::SkillE);

			EnhancedInputComponent->BindAction(QInputAction, ETriggerEvent::Started, this, &ThisClass::OnSkillInputPressed, ERInputContext::SkillQ);
			EnhancedInputComponent->BindAction(QInputAction, ETriggerEvent::Completed, this, &ThisClass::OnSkillInputReleased, ERInputContext::SkillQ);
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
	OnInputReceived.Broadcast(ERActionContext::Action_Jump);
}

void URHeroComponent::Interact()
{
	OnInputReceived.Broadcast(ERActionContext::Action_Interact);
}

void URHeroComponent::OnAttackInputPressed()
{
	UWorld* World = GetWorld();

	check(World);

	InputHoldTime[ERInputContext::Attack] = World->GetTimeSeconds();
	bFireHeavy = false;
}

void URHeroComponent::OnAttackInputTriggered()
{
	UWorld* World = GetWorld();

	check(World);

	float HoldTime = World->GetTimeSeconds() - InputHoldTime[ERInputContext::Attack];

	if (false == bFireHeavy && HoldTime > InputThresholds[ERInputStrangth::Heavy])
	{
		AResonanceCharacter* ResonanceCharacter = Cast<AResonanceCharacter>(OwnerPawn);

		if (IsValid(ResonanceCharacter))
		{
			ResonanceCharacter->RequestAttack(ERInputContext::Hold);
		}

		bFireHeavy = true;
	}
}

void URHeroComponent::OnAttackInputReleased()
{
	UWorld* World = GetWorld();

	check(World);

	float HoldTime = World->GetTimeSeconds() - InputHoldTime[ERInputContext::Attack];

	// 강공격이 이미 나갔으면 일반 공격은 실행되지않아야한다.
	if (false == bFireHeavy && HoldTime > InputThresholds[ERInputStrangth::Light])
	{
		AResonanceCharacter* ResonanceCharacter = Cast<AResonanceCharacter>(OwnerPawn);

		if (IsValid(ResonanceCharacter))
		{
			ResonanceCharacter->RequestAttack(ERInputContext::Attack);
		}
	}
}

void URHeroComponent::OnSkillInputPressed(ERInputContext InputContext)
{
	UWorld* World = GetWorld();

	check(World);

	InputHoldTime[InputContext] = World->GetTimeSeconds();
}

void URHeroComponent::OnSkillInputReleased(ERInputContext InputContext)
{
	UWorld* World = GetWorld();

	check(World);

	float HoldTime = World->GetTimeSeconds() - InputHoldTime[InputContext];

	if (HoldTime > InputThresholds[ERInputStrangth::Light])
	{
		AResonanceCharacter* ResonanceCharacter = Cast<AResonanceCharacter>(OwnerPawn);

		if (IsValid(ResonanceCharacter))
		{
			ResonanceCharacter->RequestAttack(InputContext);
		}
	}
}

void URHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerController.IsValid())
	{
		OwnerPawn = Cast<ACharacter>(PlayerController->GetPawn());
	}
}