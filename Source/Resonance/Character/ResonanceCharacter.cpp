#include "ResonanceCharacter.h"

// UE 5.
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Newly Created Files
#include "Components/RActionStateComponent.h"
#include "Components/RClimbComponent.h"
#include "Data/ResonanceEnums.h"
#include "Components/RCombatComponent.h"
#include "Components/RHeroComponent.h"
#include "Components/RHitCheckComponent.h"
#include "Components/Stat/RBaseStatComponent.h"
#include "Game/RPlayerController.h"
#include "Data/RCharacterDataTable.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AResonanceCharacter::AResonanceCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CombatComponent = CreateDefaultSubobject<URCombatComponent>(TEXT("CombatComponent"));
	HitCheckComponent = CreateDefaultSubobject<URHitCheckComponent>(TEXT("HitCheckComponent"));
	ClimbComponent = CreateDefaultSubobject<URClimbComponent>(TEXT("ClimbComponent"));
	ActionStateComponent = CreateDefaultSubobject<URActionStateComponent>(TEXT("ActionStateComponent"));
}

bool AResonanceCharacter::RequestClimb()
{
	if (IsValid(ClimbComponent))
	{
		return ClimbComponent->RequestClimb();
	}
	
	return false;
}

void AResonanceCharacter::StartClimb()
{
}

void AResonanceCharacter::RequestAttack(ERInputContext InputConext)
{
	FGameplayTag InputTag;
	switch (InputConext)
	{
	case ERInputContext::Attack :
		InputTag = FGameplayTag::RequestGameplayTag(FName("Player.Input.MouseLeft"));
		break;
	case ERInputContext::Hold:
		InputTag = FGameplayTag::RequestGameplayTag(FName("Player.Input.MouseLeft.Hold"));
		break;
	case ERInputContext::SkillE :
		InputTag = FGameplayTag::RequestGameplayTag(FName("Player.Input.E"));
		break;
	case ERInputContext::SkillQ:
		InputTag = FGameplayTag::RequestGameplayTag(FName("Player.Input.Q"));
		break;
	}

	/*PushStateGameTag(InputTag);

	if (IsValid(CombatComponent))
	{
		CombatComponent->RequestTransition(CurrentStateTags);
	}

	PopStateGameTag(InputTag);*/
}

void AResonanceCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ARPlayerController* PC = Cast<ARPlayerController>(GetController());

	if (IsValid(PC))
	{
		PC->OnCharacterDataChanged.AddUObject(this, &ThisClass::RefreshData);
	}
}

void AResonanceCharacter::BeginPlay()
{
	Super::BeginPlay();

	ARPlayerController* PC = Cast<ARPlayerController>(GetController());

	if (IsValid(PC))
	{
		URHeroComponent* HeroComponent = PC->GetHeroComponent();
		
		ensure(HeroComponent);
		
		HeroComponent->OnInputReceived.AddUObject(this, &ThisClass::OnInputReceived);
	}
	
	if (IsValid(ActionStateComponent))
	{
		FOnGameplayTagChanged& Event = ActionStateComponent->RegisterGameplayTagEvent(ERActionContext::Action_Jump);
		
		Event.AddUObject(this, &ThisClass::Jump);
	}
}

void AResonanceCharacter::RefreshData(const FRCharacterDataTable* CharacterData)
{
	if (IsValid(CombatComponent))
	{
		CombatComponent->RefreshSkillData(CharacterData);
	}
}

void AResonanceCharacter::OnInputReceived(ERActionContext ActionContext)
{
	if (IsValid(ActionStateComponent))
	{
		ActionStateComponent->PushState(ActionContext);		
	}
}