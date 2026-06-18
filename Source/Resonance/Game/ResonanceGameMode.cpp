#include "ResonanceGameMode.h"

#include "Data/ResonanceMacro.h"
#include "System/REventManager.h"
#include "Data/RMessage.h"
#include "UObject/ConstructorHelpers.h"

AResonanceGameMode::AResonanceGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AResonanceGameMode::StartPlay()
{
	Super::StartPlay();
	
	// 모든 액터들이 놓여졌을 때 알림
	REVENT_MESSAGE_NOTIFY(this, ERMessageType::StartGame);
}
