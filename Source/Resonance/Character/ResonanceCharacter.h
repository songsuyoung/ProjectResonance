#pragma once

#include "CoreMinimal.h"
#include "Character/RBaseCharacter.h"
#include "ResonanceCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URCombatComponent;
class URHitCheckComponent;
struct FRCharacterDataTable;
enum class ERSkillType :uint8;
enum class ERInputContext : uint8;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AResonanceCharacter : public ARBaseCharacter
{
	GENERATED_BODY()

public:
	AResonanceCharacter();


	void RequestAttack(ERInputContext InputConext);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class URCombatComponent* GetCombatComponent() const { return CombatComponent; }

	FORCEINLINE class URHitCheckComponent* GetHitCheckComponent() const { return HitCheckComponent; }
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	void RefreshData(const FRCharacterDataTable* CharacterData);

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<URCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<URHitCheckComponent> HitCheckComponent;


};

