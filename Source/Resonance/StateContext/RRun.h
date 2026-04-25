#pragma once

#include "CoreMinimal.h"
#include "StateContext/RStateContext.h"
#include "RRun.generated.h"

class ACharacter;
class UCharacterMovementComponent;
UCLASS()
class RESONANCE_API URRun : public URStateContext
{
	GENERATED_BODY()
	
public:
	URRun();
	virtual void Initialize(AActor* InOwner) override;
	virtual void Execute() override;
	virtual void OnExit() override;
	
protected:
	// 달리기 최대 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Run|Speed")
	float MaxSpeed;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> CharacterRef;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UCharacterMovementComponent> CMCRef;
	
	UPROPERTY(Transient)
	float OriginalSpeed;
};
