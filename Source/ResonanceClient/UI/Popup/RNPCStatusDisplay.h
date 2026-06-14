#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/RMessageReceiver.h"
#include "RNPCStatusDisplay.generated.h"

class UTextBlock;
struct FRUIStatInfo;
enum class ERStatType : uint8;

UCLASS()
class RESONANCECLIENT_API URNPCStatusDisplay : public UUserWidget, public IRMessageReceiver
{
	GENERATED_BODY()
	
public:
	URNPCStatusDisplay(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnMessage(ERMessageType Type, FRMessage* Message = nullptr) override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	void InitStat(const TArray<FRUIStatInfo>& StatInfos);	
	void UpdateStat(ERStatType StatType, int32 NewValue);
protected:
	
	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Stat;
	
	UPROPERTY(Transient)
	FString StatText;
	
	// string format용도
	FStringFormatNamedArguments Args;
};
