#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "RRootWindow.generated.h"

class URCommonActivatableWidget;
class UCommonActivatableWidgetStack;
class URUIWindowLoadMethod;

USTRUCT()
struct FRPoolContainer
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(Transient)
	TArray<TObjectPtr<URCommonActivatableWidget>> Container;
};

UCLASS()
class RESONANCECLIENT_API URRootWindow : public UCommonUserWidget
{
	GENERATED_BODY()
		
	virtual void NativeConstruct() override;

public:
	virtual URCommonActivatableWidget* GetOrCreateInstance(UClass* Class, const FGameplayTag& LayerTag);

protected:

	URCommonActivatableWidget* GetOrCreateInstanceInternal(UClass* Class, const FGameplayTag& LayerTag);

protected:

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<URUIWindowLoadMethod> WindowLoadMethodClass;
	
protected:
	
	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> Root;
	
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> LayerMap;

	UPROPERTY(Transient)
	TMap<FName, FRPoolContainer> PoolingContainer;
};
