#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPlayerController.generated.h"

class URHeroComponent;
struct FRCharacterDataTable;

UCLASS()
class RESONANCE_API ARPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARPlayerController(const FObjectInitializer& ObjectInitializer);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterDataChanged,const FRCharacterDataTable*)
	FOnCharacterDataChanged OnCharacterDataChanged;

	// Pawn이 변경될 때 마다 데이터가 변경되어 진다.
	void RefreshData(const FName& CharacterID);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<URHeroComponent> HeroComponent;

};
