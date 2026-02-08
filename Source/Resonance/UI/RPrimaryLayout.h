#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/RMessageReceiver.h"
#include "RPrimaryLayout.generated.h"

UCLASS()
class RESONANCE_API URPrimaryLayout : public UUserWidget, public IRMessageReceiver
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:

	virtual void OnMessage(ERMessageType Type, FRMessage* Message = nullptr) override;

};
