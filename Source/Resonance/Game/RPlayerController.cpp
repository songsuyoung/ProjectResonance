#include "Game/RPlayerController.h"

// UE 5.

// New Created Files...
#include "Components/RHeroComponent.h"
#include "Character/ResonanceCharacter.h"
#include "System/RDataManager.h"
#include "Data/RCharacterDataTable.h"

ARPlayerController::ARPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeroComponent = CreateDefaultSubobject<URHeroComponent>("HeroComponent");
}

// PlayerController는 입력처리와 캐릭터 사이의 중간다리 역할
void ARPlayerController::RefreshData(const FName& CharacterID)
{
	// 플레이어블한 캐릭터의 ID값으로 데이터 테이블 가져온다.
	URDataManager* DataManager = URDataManager::Get(this);

	check(DataManager);

	// Hero Component 정리
	const FRCharacterDataTable* CharacterDataTable = DataManager->GetDataTableRow<FRCharacterDataTable>(ERDataTableType::CharacterData, CharacterID);

	if (nullptr != CharacterDataTable)
	{
		OnCharacterDataChanged.Broadcast(CharacterDataTable);
	}
}

void ARPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 첫 시작이 101번
	RefreshData(FName(TEXT("101")));
	
}

void ARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsValid(HeroComponent))
	{
		HeroComponent->SetupInputComponent();
	}
}