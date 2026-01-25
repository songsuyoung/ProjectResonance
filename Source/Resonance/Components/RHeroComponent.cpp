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
	bWantsInitializeComponent = true;
}

void URHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARPlayerController>(GetOwner());

	if (PlayerController.IsValid())
	{
		OwnerPawn = Cast<ACharacter>(PlayerController->GetPawn());

		PlayerController->OnCharacterDataChanged.AddUObject(this, &ThisClass::RefreshData);
	}
}

void URHeroComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void URHeroComponent::RefreshData(const FRCharacterDataTable* Data)
{
	if (nullptr != Data)
	{
		InputHoldTime = Data->InputHoldTime;
	}
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
			EnhancedInputComponent->BindAction(DefaultInputAction, ETriggerEvent::Started, this, &ThisClass::OnAttackInputPressed, ERSkillType::Default);
			EnhancedInputComponent->BindAction(DefaultInputAction, ETriggerEvent::Completed, this, &ThisClass::OnAttackInputReleased);
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

void URHeroComponent::Attack(ERSkillType SkillType)
{
	if (OwnerPawn.IsValid())
	{
		AResonanceCharacter* LocalCharacter= Cast<AResonanceCharacter>(OwnerPawn);

		if (IsValid(LocalCharacter))
		{
			URCombatComponent* CombatComponent = LocalCharacter->GetCombatComponent();

			if (IsValid(CombatComponent))
			{
				CombatComponent->Attack(SkillType);
			}
		}
	}
}

void URHeroComponent::OnAttackInputPressed(ERSkillType SkillType)
{
	UWorld* World = GetWorld();

	check(World);

	AttackInputHoldTime = World->GetTimeSeconds();

	if (PendingSkillType == ERSkillType::None)
	{
		PendingSkillType = SkillType;
	}
}

void URHeroComponent::OnAttackInputReleased()
{
	if (false == InputHoldTime.Contains(PendingSkillType))
	{
		return;
	}

	UWorld* World = GetWorld();

	check(World);

	const float& HoldTime = World->GetTimeSeconds() - AttackInputHoldTime;

	if (HoldTime >= InputHoldTime[PendingSkillType])
	{
		// 기본 공격 -> 강공격인지 확인, HoldTime이 강공격보다 클경우에 강공격 실행


		// 공격 실행 
		Attack(PendingSkillType);
		PendingSkillType = ERSkillType::None;
	}

}
