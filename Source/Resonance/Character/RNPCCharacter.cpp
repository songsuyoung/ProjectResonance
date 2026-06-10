#include "RNPCCharacter.h"

// UE 5.
#include "Components/SplineComponent.h"

// 
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
	AIControllerClass = ARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARNPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARNPCCharacter::VisitRegion(const FName& RegionID)
{
	CurrentVisitedRegion = RegionID;
	
	// AIController에게도 전달해야한다.
	// 의존성을 줄이는 방법은 델리게이트로 둘다 연결하고 호출하게 하는 방법이 가장 좋아보인다.
	// EventManager를 관리하는 것도 고려해본다.
	ARAIController* AIController = GetController<ARAIController>();
	
	if (IsValid(AIController))
	{
		AIController->OnRegionVisited.Broadcast(RegionID);
		
		FRNPCReleaseMessage Msg(ID, RegionID);
		REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::EnterRegion, Msg);
	}
}

FName ARNPCCharacter::ConsumeNextRegion()
{
	if (PendingRegions.IsEmpty())
	{
		// 전부 다 소진함.
		return FName();
	}
	FName NextRegion = PendingRegions[0];
	
	// 0번째 인덱스 삭제 
	PendingRegions.RemoveAt(0);
	
	return NextRegion;
}

