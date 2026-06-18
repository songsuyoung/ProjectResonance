#include "System/RUIManagerBase.h"

#include "RGameInstance.h"

URUIManagerBase* URUIManagerBase::Get(UObject* Context)
{
	UWorld* World = Context->GetWorld();
	
	check(World);
	
	
	URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());
	
	check(GameInstance);
	
	return GameInstance->GetUIManager();
}
