#pragma once

#include "CoreMinimal.h"
#include "RCameraCharacter.h"
#include "Character/RBaseCharacter.h"
#include "ResonanceCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URCombatComponent;
class URHitCheckComponent;
class URClimbComponent;
struct FRCharacterDataTable;
enum class ERSkillType :uint8;
enum class ERInputContext : uint8;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
enum class ERActionContext : uint8;
UCLASS(config=Game)
class AResonanceCharacter : public ARCameraCharacter
{
	GENERATED_BODY()

public:
	AResonanceCharacter();

	bool RequestClimb();
	void RequestAttack(ERInputContext InputConext);;
public:
	FORCEINLINE class URCombatComponent* GetCombatComponent() const { return CombatComponent; }

	FORCEINLINE class URHitCheckComponent* GetHitCheckComponent() const { return HitCheckComponent; }
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	void RefreshData(const FRCharacterDataTable* CharacterData);
	void StartClimb();
	void OnInputReceived(ERActionContext ActionContext);
	
protected:
	
	// 캐릭터의 전체 상태를 관리할 예정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<URActionStateComponent> ActionStateComponent;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<URCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<URHitCheckComponent> HitCheckComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Climb")
	TObjectPtr<URClimbComponent> ClimbComponent;
};

