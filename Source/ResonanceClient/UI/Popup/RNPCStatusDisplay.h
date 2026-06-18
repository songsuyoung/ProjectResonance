#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/RMessageReceiver.h"
#include "RNPCStatusDisplay.generated.h"

class UTextBlock;
struct FRStatInfo;
enum class ERStatType : uint8;
enum class EREmotionState : uint8;
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
	void InitStat(const TArray<FRStatInfo>& StatInfos);	
	void UpdateStat(ERStatType StatType, int32 NewValue);
	void UpdateEmotion(EREmotionState EmotionState);
protected:
	
	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Stat;
	
	UPROPERTY(Transient)
	FString StatText;

	UPROPERTY(Transient)
	FName OwnerID;
	
	// string format용도
	FStringFormatNamedArguments Args;
};
