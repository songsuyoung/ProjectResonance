#include "ResonanceGameMode.h"
#include "Character/ResonanceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AResonanceGameMode::AResonanceGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
