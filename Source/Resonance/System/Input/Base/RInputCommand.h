#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RInputCommand.generated.h"

class APawn;
enum class ETriggerEvent : uint8;

UCLASS(Abstract)
class RESONANCE_API URInputCommand : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(APawn* InTargetPawn) { TargetPawn = InTargetPawn; }
	ETriggerEvent GetTriggerEvent() { return TriggerEvent;  }

	// 메타 프로그래밍을 통해서, 다른 매개변수 값을 처리해야한다.
	virtual void Exec() PURE_VIRTUAL(URInputCommand::Exec, ;)

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	ETriggerEvent TriggerEvent;

	UPROPERTY(Transient)
	TWeakObjectPtr<APawn> TargetPawn;
};
