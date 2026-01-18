#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RSkillBase.generated.h"

class UAnimMontage;
DECLARE_MULTICAST_DELEGATE(FAttackEventDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCooldownEventDelegate, URSkillBase*)

// 단일 책임의 원칙에 따라, 스킬 내부에서 쿨타임, 공격시작/끝/진행 중 모두를 관리한다.
// 블루프린트로 만들 수 있음.
UCLASS(Abstract)
class RESONANCE_API URSkillBase : public UObject
{
	GENERATED_BODY()

public:
	URSkillBase();

	void Init(ACharacter* InOwner);
	bool CanUseSkill() { return bCanBeActivated; }
	bool IsPlaying() const;
	bool CanReserveCombo();

	// 구조체로 Context 넘길 예정
	bool TryAttack();

	// CombatComponent 에서 활성화 스킬의 Tick을 관리해주어야 한다.
	// 쿨타임 동작	
	// 비활성화된 스킬에서 동작
	// Tick -> 타이머로 변경 전 성능 체크 확인해볼 필요가 있음.
	// 타이머의 경우 월드를 이동하거나, 호출되지않는 경우도 존재하는지의 여부를 확인해봐야하기 때문임.
	virtual void Tick(const float& DeltaTime);

protected:
	
	bool IsInputValid(const double& NowTime);
	void Execute();
	
protected:
	virtual void BindEvents();
	virtual void PrepareAttack();
	virtual void EndAttack(UAnimMontage* PlayingAnimMontage, bool bInterrupted);
	virtual void Attack();

public:

	// 선언은 한번만 진행한다.
	// 공격과 관련된 델리게이트임을 명시
	FAttackEventDelegate OnAttackStarted;
	FAttackEventDelegate OnAttackCompleted;
	FOnCooldownEventDelegate OnCooldownEventDelegate;

protected:

	// 스킬시 사용되어지는 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	TObjectPtr<UAnimMontage> SkillMontage;

	// 스킬 활성화-비활성화를 위한 쿨타임
	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	float CoolTime;

	// 조작감을 주기 위한 입력 임계값
	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	float InputThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	FFloatRange ComboRangeThreshold;

protected:

	// 쿨타임 누적 시간
	UPROPERTY(Transient)
	float CurrentCooldownTimer;

	// 스킬을 시도한 시간
	UPROPERTY(Transient)
	double LastAttackAtteptTime;

	UPROPERTY(Transient)
	uint8 bCanBeActivated : 1;

	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAnimInstance> CachedAnimInstance;
};
