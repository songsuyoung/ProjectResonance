#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RCombatComponent.generated.h"

class ARWeaponBase;
class URSkillBase;
enum class ERSkillType : uint8;
struct FRCharacterDataTable;

// 블루프린트화 시킬 수 있다.
USTRUCT(BlueprintType)
struct FRSkillTransition
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|To")
	ERSkillType ToAction;

	// 발동 조건
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Condition")
	FGameplayTagContainer RequiredTags;

	// 금지 조건
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Condition")
	FGameplayTagContainer BlockedTags;
};

USTRUCT()
struct FRSkillContainer
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
	TArray<TObjectPtr<URSkillBase>> Skills;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESONANCE_API URCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URCombatComponent();

	ARWeaponBase* GetWeapon() const { return Weapon;  }
	void PlayNextCombo();

	/* Set: 스킬 */
	void RefreshSkillData(const FRCharacterDataTable* Data);
	void RequestTransition(const FGameplayTagContainer& CurrentTags);
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/* 전투 */
	void Attack(URSkillBase* Skill);
	void OnCooldownEventDelegate(URSkillBase* Skill);
	void ExecuteAttack(URSkillBase* Skill);
	void TryReserveNextCombo(URSkillBase* Skill);
	void OnAttackCompleted(const FGameplayTag& EndSkillTag);
	void OnAttackStarted(const FGameplayTag& ActiveSkillTag);
	
protected:
	/* 무기 */
	void InitializeWeapon();
	void UpdateWeaponEquipState();
	void UpdateWeaponState();
protected:

	/*
	* 프로토타입
	* TODO: 리팩토링 필요, 데이터 구조로 변경해야함. (데이터 매니저로 얻어올 수있도록)
	* 무기 변경이 언제든 가능한 구조로 유연하게 수정 예정
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Stat|Class|Weapon")
	TSubclassOf<ARWeaponBase> WeaponClass;

	// 무기를 장착하고 나서, 활성화되어지는 시간
	// 공격이 없으면, 무기를 넣는다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Weapon")
	float ActivationDelay;

	// 무기를 넣고나서, 무기가 비활성화 되어지는 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Weapon")
	float DeactivationDelay;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Skill")
	TArray<FRSkillTransition> Transition;

protected:

	UPROPERTY(Transient)
	TObjectPtr<ARWeaponBase> Weapon;

	// 쿨타임 관리용
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<URSkillBase>> ActiveSkills;

	// 콤보용
	UPROPERTY(Transient)
	TWeakObjectPtr<URSkillBase> PendingComboSkill;

	// 현재활성화 스킬
	UPROPERTY(Transient)
	TWeakObjectPtr<URSkillBase> CurrentActiveSkill;

	// 이를 일반 TMap으로 구분,
	UPROPERTY(Transient)
	TMap<ERSkillType, TObjectPtr<URSkillBase>> SkillSlots;

	// 마지막 공격 이후에 공격하지 않은 채로 경과한 시간
	// 만약, 경과한 시간이 ActivationDelay 보다 클 경우, 무기를 넣을 예정
	UPROPERTY(Transient)
	double LastAttackTime;

	UPROPERTY(Transient)
	double LastUnequipTime;

	// 현재는 true/false로 가능하지만, 스킬이 늘어나게 된다면
	// bit 로 각 스킬의 공격이 끝났는지 아닌지
	// 위 ActiveSkill은 스킬을 발동하자 마자 바로 쿨타임을 돌도록 설계되어있어서
	// 공격 완료와 맞지않는다.
	UPROPERTY(Transient)
	int32 ActiveAttackCount;

	// TODO: 수정 예정 
	// 이유: 일반 공격에 대해서 콤보를 어떻게 구현할지 알앤디
	UPROPERTY(Transient)
	int32 CurrentComboIndex;
};
