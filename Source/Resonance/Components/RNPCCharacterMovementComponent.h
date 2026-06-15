#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RNPCCharacterMovementComponent.generated.h"

class URBaseStatComponent;
enum class ERStatType : uint8;
UCLASS()
class RESONANCE_API URNPCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	URNPCCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

protected:
	
	void HandleStatChanged(ERStatType StatType, float MaxStat, float Stat);

protected:
	// TODO: 추후에 데이터 테이블화 시킬 것.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StaminaDrainInterval;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URBaseStatComponent> StatComponent;
	
	UPROPERTY(Transient)
	float CachedStamina;
	
	UPROPERTY(Transient)
	float StaminaDrainTimer;
};
