#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RBaseStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URBaseStatComponent();


protected:

	/* HP : 일반 체력 */
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float MaxHP;

	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float CurrentHP;

	/* Stamina : 달리기, 수영, 날기 등 제한된 활성화 값 */
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float MaxStamina;

	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float CurrentStamina;

};
