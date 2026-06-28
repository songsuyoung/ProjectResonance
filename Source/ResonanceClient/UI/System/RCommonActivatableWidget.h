#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "RCommonActivatableWidget.generated.h"

UCLASS()
class RESONANCECLIENT_API URCommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CommonUI|Settings")
	FUIInputConfig InputConfig;
};
