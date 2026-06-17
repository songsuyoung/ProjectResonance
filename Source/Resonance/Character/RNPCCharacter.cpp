#include "RNPCCharacter.h"

// UE 5.
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"

// 
#include "Components/Emotion/REmotionComponent.h"
#include "Components/Stat/RBaseStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NPC/System/RAIController.h"
#include "System/ResonanceMacro.h"
#include "System/REventManager.h"
#include "System/RMessage.h"

ARNPCCharacter::ARNPCCharacter()
	: Super()
	, CurrentVisitedRegion()
	
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	EmotionComponent = CreateDefaultSubobject<UREmotionComponent>(TEXT("EmotionComponent"));
	EmotionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Blueprints/UI/WidgetComponent/WBP_StatusDisplay"));
	if (WidgetClassFinder.Succeeded())
	{
		EmotionWidgetComponent->SetWidgetClass(WidgetClassFinder.Class);
	}
	
	EmotionWidgetComponent->SetupAttachment(GetMesh(), TEXT("head"));
	EmotionWidgetComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	EmotionWidgetComponent->SetDrawSize(FVector2D(200.0f, 100.0f));
	EmotionWidgetComponent->SetRelativeRotation(FRotator(0.f, 90.0f, -90.0f));
	
	BaseStatComponent = CreateDefaultSubobject<URBaseStatComponent>(TEXT("BaseStatComponent"));
	
	AIControllerClass = ARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
}

void ARNPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OnRegionEntered.AddUObject(this, &ThisClass::HandleRegionEntered);
	OnRegionExited.AddUObject(this, &ThisClass::HandleRegionExited);
}

void ARNPCCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	OnRegionEntered.RemoveAll(this);
	OnRegionExited.RemoveAll(this);
}

void ARNPCCharacter::HandleRegionEntered()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARNPCCharacter::HandleRegionExited()
{
	if (IsValid(BaseStatComponent))
	{
		BaseStatComponent->RecoverStatOnSpawn(ERStatType::Stamina, 0.2f);
		
		// Stat 계산을 수행해야한다. 선호하는 지역이라면, 스탯을 증가시키고,
		// 신호하지 않는 지역이라면 스탯을 감소시킨다.
		TMap<FName, float> PreferredRegions, DislikedRegions;
		
		BaseStatComponent->GetRegionPreferences(PreferredRegions, DislikedRegions);
		
		const auto PreferenceRegionPercentage = PreferredRegions.Find(CurrentVisitedRegion);
		
		float MaxValue = BaseStatComponent->GetMaxStatValue(ERStatType::Mood);
		float CurValue = BaseStatComponent->GetCurrentStatValue(ERStatType::Mood);
		if (nullptr != PreferenceRegionPercentage)
		{
			//Mood도 증가시킴.
			float NewValue = CurValue + (MaxValue - MaxValue * (*PreferenceRegionPercentage)); // MaxValue * 0.7 => MaxValue - 70% => 더해줌.
			BaseStatComponent->UpdateStat(ERStatType::Mood, NewValue);
		}
		
		// 민액 싫어하는 곳을 갔다면, 선호도를 떨어트려야 한다.
		const auto DislikedRegionPercentage = DislikedRegions.Find(CurrentVisitedRegion);
		
		if (nullptr != DislikedRegionPercentage)
		{
			float NewValue = CurValue - (MaxValue - (MaxValue * (*DislikedRegionPercentage)));
			BaseStatComponent->UpdateStat(ERStatType::Mood, NewValue);
		}
	}
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ARNPCCharacter::VisitRegion(const FName& RegionID)
{
	if (false == IsValid(EmotionComponent))
	{
		return;
	}
	
	// 이미 방문한 경우에는 변경하지 않는다.
	if (CurrentVisitedRegion == RegionID)
	{
		return;
	}
	
	if (PendingRegions.IsEmpty())
	{
		// 전부 다 소진함.
		return;
	}
	
	FName NextRegion = PendingRegions[0];
	
	if (RegionID != NextRegion)
	{
		return;
	}
	
	CurrentVisitedRegion = RegionID;
	PendingRegions.RemoveAt(0);
	float DurationTime = EmotionComponent->GetStayDuration(RegionID);

	FRNPCEnterRegion Msg(ID, RegionID, DurationTime);
	REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::EnterRegion, Msg);
}

FName ARNPCCharacter::PeekNextRegion()
{
	if (PendingRegions.IsEmpty())
	{
		// 전부 다 소진함.
		return FName();
	}
	
	return PendingRegions[0];
}

