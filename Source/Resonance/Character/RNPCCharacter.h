#pragma once

#include "CoreMinimal.h"
#include "RCameraCharacter.h"
#include "Character/RBaseCharacter.h"
#include "RNPCCharacter.generated.h"

class URActionData;
class UStateTreeAIComponent;
class USplineComponent;
class UREmotionComponent;
class UWidgetComponent;
class UUserWidget;
class APlayerController;
DECLARE_MULTICAST_DELEGATE(FOnRegionEntered);
DECLARE_MULTICAST_DELEGATE(FOnRegionExited);

UCLASS()
class RESONANCE_API ARNPCCharacter : public ARCameraCharacter
{
	GENERATED_BODY()
	
public:
	ARNPCCharacter();
	
	USplineComponent* GetSplineComponent() const { return SplineComponent; }
	UREmotionComponent* GetEmotionComponent() const { return EmotionComponent; }
	FName GetCurrentVisitedRegion() const { return CurrentVisitedRegion; }
	void SetID(FName NPCID, FName HomeGroundID) { Super::SetID(NPCID); CurrentVisitedRegion = HomeGroundID; }
	bool HasNextRegion() { return false == PendingRegions.IsEmpty(); }
	void SetNextRegions(const TArray<FName>& NewNextRegions) { PendingRegions = NewNextRegions; }
	FName GetPreferenceLocationID() { return TEXT("101"); }
	float GetBaseDuration() { return 60.f; }
public:
	
	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void EndViewTarget(APlayerController* PC) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void VisitRegion(const FName& RegionID);
	FName PeekNextRegion();
public:

	FOnRegionEntered OnRegionEntered;
	FOnRegionExited OnRegionExited;
	
protected:
	void HandleRegionEntered();
	void HandleRegionExited();

protected:
	
	// TODO: 감정으로 다음 지역결정
	// 현재는 자기 스스로 정의하지 못하지만. 나중에, 스탯 붙고나서 자율 적으로 고민할 수 있도록 코딩할 예정.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Path|Settings")
	TArray<FName> PendingRegions;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Path|Settings")
	TObjectPtr<USplineComponent> SplineComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Emotion|Settings")
	TObjectPtr<UREmotionComponent> EmotionComponent;
	
protected:
	// 현재 방문했던 곳은 다시 들리지 않는다. 
	// 다시 들리는 경우는 어느정도 나왔다가 다시 돌아가는 경우가 있지만. 선택할 때 반복하지않도록한다.(기획)
	UPROPERTY(Transient)
	FName CurrentVisitedRegion;
	
	UPROPERTY(Transient)
	FName HomeID;
};
