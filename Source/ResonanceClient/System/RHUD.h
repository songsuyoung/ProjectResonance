#pragma once

#include "CoreMinimal.h"
#include "UI/RHUDBase.h"
#include "RHUD.generated.h"
class URRootWindow;

UCLASS()
class RESONANCECLIENT_API ARHUD : public ARHUDBase
{
	GENERATED_BODY()

public:

protected:
	virtual void Init() override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "RootWidget")
	TSubclassOf<URRootWindow> RootWidgetClass;
};
