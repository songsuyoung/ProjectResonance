#pragma once

#include "CoreMinimal.h"
#include "Character/RBaseCharacter.h"
#include "RNPCCharacter.generated.h"
class URActionData;
class UStateTreeAIComponent;
class USplineComponent;

UCLASS()
class RESONANCE_API ARNPCCharacter : public ARBaseCharacter
{
	GENERATED_BODY()
	
public:
	ARNPCCharacter();
	
	virtual void BeginPlay() override;
	
public:
	USplineComponent* GetSplineComponent() const { return SplineComponent; }
	void VisitRegion(const FName& RegionID);
	FName GetCurrentVisitedRegion() const { return CurrentVisitedRegion; }
	FName ConsumeNextRegion();
	
protected:
	
	// TODO: 변경해야함. 임시로 경로를 지정해준다
	// 이 부분은 추후에 개발될 예정
	// 현재는 자기 스스로 정의하지 못하지만. 나중에, 스탯 붙고나서 자율 적으로 고민할 수 있도록 코딩할 예정.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Path")
	TArray<FName> PendingRegions;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings|Path")
	TObjectPtr<USplineComponent> SplineComponent;
	
	// 현재 방문했던 곳은 다시 들리지 않는다. 
	// 다시 들리는 경우는 어느정도 나왔다가 다시 돌아가는 경우가 있지만. 선택할 때 반복하지않도록한다.(기획)
	UPROPERTY(Transient)
	FName CurrentVisitedRegion;
};
