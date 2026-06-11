#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RRegionVolume.generated.h"

UCLASS()
class RESONANCE_API ARRegionVolume : public AVolume
{
	GENERATED_BODY()

public:
	ARRegionVolume();
	
	// BeginPlay 함수에서는 NPC를 스폰한다.
	virtual void BeginPlay() override;
	
	// OnOverlapBegin 함수 내부에서는 오버랩된 NPC에게 현재 Region이 최근 방문지임을 전달한다.
	UFUNCTION()
	void OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
protected:
	
	UPROPERTY(EditAnywhere, Category = "Region|Settings")
	FName RegionID;
	
protected:
	
	UPROPERTY(Transient)
	TSet<TWeakObjectPtr<ACharacter>> OverlappedCharacters;
	
};
