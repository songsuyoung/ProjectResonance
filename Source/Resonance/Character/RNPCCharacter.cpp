#include "RNPCCharacter.h"

// UE 5.
#include "Components/SplineComponent.h"

// 
#include "Components/REmotionComponent.h"
#include "Components/StateTreeAIComponent.h"
#include "Data/ResonanceEnums.h"
#include "Data/ResonanceMacro.h"
#include "NPC/System/RAIController.h"
#include "System/REventManager.h"
#include "System/RMessage.h"

ARNPCCharacter::ARNPCCharacter()
	: Super()
	, CurrentVisitedRegion()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	EmotionComponent = CreateDefaultSubobject<UREmotionComponent>(TEXT("EmotionComponent"));
	
	AIControllerClass = ARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
	// 밑에 히든하기 전에 액션일 취할 필요가 있을 때 여기서 해주는게 맞다.
	// 왜냐하면 캐릭터 자체의 움직임이기 때문임.
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARNPCCharacter::HandleRegionExited()
{
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
	float DurationTime = EmotionComponent->GetStayDuration();

	FRNPCEnterRegion Msg(ID, RegionID, DurationTime);
	REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::EnterRegion, Msg);
}

FName ARNPCCharacter::ConsumeNextRegion()
{
	if (PendingRegions.IsEmpty())
	{
		// 전부 다 소진함.
		return FName();
	}
	
	return PendingRegions[0];
}

