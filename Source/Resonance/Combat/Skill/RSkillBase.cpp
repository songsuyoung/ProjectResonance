#include "Combat/Skill/RSkillBase.h"

// UE
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

// Newly Created Files...

URSkillBase::URSkillBase()
	: Super()
	, CurrentCooldownTimer(0.f)
	, LastAttackAtteptTime(0.f)
	, bCanBeActivated(false)
	, OwnerCharacter(nullptr)

{

}

void URSkillBase::Init(ACharacter* InOwner)
{
	// 데이터 초기화
	OwnerCharacter = InOwner;
	CurrentCooldownTimer = 0.f;
	LastAttackAtteptTime = 0.f;
	bCanBeActivated = true;

	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();

	if (IsValid(MeshComponent))
	{
		CachedAnimInstance = MeshComponent->GetAnimInstance();
	}
}

bool URSkillBase::TryAttack()
{
	UWorld* World = GetWorld();

	check(World);

	const double& CurrentTimeSeconds = World->GetTimeSeconds(); //현재 시간초 가져온다.

	// 조작감을 위해서 입력
	if (false == IsInputValid(CurrentTimeSeconds))
	{
		// 유효하지 않은 입력
		return false;
	}

	// 유효한 입력이기 때문에, 마지막으로 공격을 시도한 시간 저장
	LastAttackAtteptTime = CurrentTimeSeconds;

	if (false == bCanBeActivated)
	{
		return false;
	}

	Execute();

	return true;
}

void URSkillBase::BindEvents()
{
	if (IsValid(SkillMontage))
	{
		FOnMontageEnded OnMonstageEnded;

		OnMonstageEnded.BindUObject(this, &ThisClass::EndAttack);

		if (CachedAnimInstance.IsValid())
		{
			CachedAnimInstance->Montage_SetEndDelegate(OnMonstageEnded, SkillMontage);
		}
	}
}

bool URSkillBase::IsInputValid(const double& NowTime)
{
	bool bValidInput = (NowTime - LastAttackAtteptTime) > InputThreshold;

	return bValidInput;
}

void URSkillBase::Execute()
{
	// 스킬을 사용하고 쿨타임이 돌 수 있도록 변경
	bCanBeActivated = false;

	// 공격 시작 전
	PrepareAttack();
	// 공격 진행
	Attack();
}

bool URSkillBase::IsPlaying() const
{
	return CachedAnimInstance->IsAnyMontagePlaying();
}

bool URSkillBase::CanReserveCombo()
{
	bool bResult = false;
	if (CachedAnimInstance.IsValid())
	{
		const float& Position = CachedAnimInstance->Montage_GetPosition(SkillMontage);
		const float& Length = SkillMontage->GetPlayLength();

		if (Length > 0.0f)
		{
			const float& CurrentRatio = Position / Length;

			bResult = ComboRangeThreshold.Contains(CurrentRatio);
		}
	}

	return bResult;
}

void URSkillBase::Tick(const float& DeltaTime)
{
	if (bCanBeActivated)
	{
		// 아직 사용하지 않았지만, 사용 가능한 스킬이다.
		// 즉, 쿨타임이 끝났다.
		return;
	}

	CurrentCooldownTimer += DeltaTime;
	
	// 실제 계속 달라지는 월드의 시간에 마지막으로 공격이 진행된 시간 확인
	// 장착한 경우 -> 공격 후 일정 시간 뒤 UnEquip 예정
	bCanBeActivated = CurrentCooldownTimer >= CoolTime;

	if (bCanBeActivated)
	{
		// 쿨타임이 전부 돌았음.
		// UI 활성화 해준다.
		OnCooldownEventDelegate.Broadcast(this);
	}

}

void URSkillBase::PrepareAttack()
{
	OnAttackStarted.Broadcast();
}

void URSkillBase::Attack()
{
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->PlayAnimMontage(SkillMontage);

		BindEvents();
	}
}

void URSkillBase::EndAttack(UAnimMontage* PlayingAnimMontage, bool bInterrupted)
{
	if (PlayingAnimMontage == SkillMontage)
	{
		OnAttackCompleted.Broadcast();
	}
}