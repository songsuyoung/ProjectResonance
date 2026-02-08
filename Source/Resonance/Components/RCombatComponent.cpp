#include "RCombatComponent.h"

// UE 5.
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

// Newly Created File 
#include "Combat/Skill/RSkillBase.h"
#include "Weapon/RWeaponBase.h"
#include "System/RDataManager.h"
#include "Data/ResonanceEnums.h"
#include "Data/RSkillDataTable.h"
#include "Data/RCharacterDataTable.h"
#include "Game/RPlayerController.h"
#include "Character/RBaseCharacter.h"
#include "System/REventManager.h"
#include "System/RMessage.h"

URCombatComponent::URCombatComponent()
	: Super()
	, Weapon(nullptr)
	, ActiveSkills()
	, SkillSlots()
	, ActiveAttackCount(0)
	, CurrentComboIndex(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void URCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeWeapon();
}

void URCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 배열 순회 중에 변경이 발생할 수 있기 때문에 뒤부터 실행
	for (int SkillIndex = ActiveSkills.Num() - 1; SkillIndex >= 0; --SkillIndex)
	{
		URSkillBase* ActiveSkill = ActiveSkills[SkillIndex].Get();

		if (false == IsValid(ActiveSkill))
		{
			ActiveSkills.RemoveAtSwap(SkillIndex);
			continue;
		}

		ActiveSkill->Tick(DeltaTime);
	}


	// 장착되어져 있을 때와 관련되어진다. (모두 공격이 완료 된 이후에 실행해야한다!)
	// Weapon 자체가 활성화 되어져 있다면 아래 로직처리들을 수행
	// bAttackCompleted & 0 => 0이 되는 경우를 확인해야한다.
	if (IsValid(Weapon) && Weapon->IsWeaponActive() && ActiveAttackCount <= 0)
	{
		UpdateWeaponEquipState();
		UpdateWeaponState();
	}
}

void URCombatComponent::InitializeWeapon()
{
	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters Params;

	// URCombatComponent 의 Owner값이 Actor의 Owner로 지정
	Params.Owner = GetOwner();
	// 마켓에서 가져온 메시를 어떻게 처리할지 확인해야한다.
	Weapon = World->SpawnActor<ARWeaponBase>(WeaponClass, Params);
}

void URCombatComponent::UpdateWeaponEquipState()
{
	// Weapon 없거나, 이미 장비 해제된 경우에는 Tick이 돌지않도록 한다.
	if (false == Weapon->IsEquipped())
	{
		return;
	}

	UWorld* World = GetWorld();
	
	check(World);

	const double& NowTime = World->GetTimeSeconds();

	bool bShouldUnequip = (NowTime - LastAttackTime) > ActivationDelay;
	
	if (bShouldUnequip)
	{
		Weapon->UnEquip();
		LastUnequipTime = NowTime;
	}

}

void URCombatComponent::UpdateWeaponState()
{
	// Weapon 없거나, 이미 장비 해제된 경우에는 Tick이 돌지않도록 한다.
	// 장비를 차고 있으면 return, EquipState 진행 후에 UpdateWeaponState를 진행해야하기 때문이다.
	if (Weapon->IsEquipped())
	{
		return;
	}

	UWorld* World = GetWorld();

	check(World);

	const double& NowTime = World->GetTimeSeconds();

	bool bShouldDeactivate = (NowTime - LastUnequipTime) > DeactivationDelay;

	if (bShouldDeactivate)
	{
		Weapon->DeactivateWeapon();
	}
}

void URCombatComponent::RefreshSkillData(const FRCharacterDataTable* Data)
{
	if (nullptr != Data)
	{
		const TMap<ERSkillType, FRComboSkillContainer>& ComboSkillContainer = Data->SkillContainer;

		URDataManager* DataManager = URDataManager::Get(this);

		check(DataManager);

		for (const TPair<ERSkillType, FRComboSkillContainer>& Iterator : ComboSkillContainer)
		{
			for(const FName& IDIterator : Iterator.Value.SkillIDs)
			{
				FRSkillDataTable* SkillData = DataManager->GetDataTableRow<FRSkillDataTable>(ERDataTableType::SkillData, IDIterator);

				if (nullptr != SkillData)
				{
					URSkillBase* SkillBase = NewObject<URSkillBase>(this, SkillData->SkillClass);

					if (IsValid(SkillBase))
					{
						ACharacter* Character = Cast<ACharacter>(GetOwner());

						if (IsValid(Character))
						{
							// 스킬 초기화
							SkillBase->Init(Character);
							SkillBase->OnCooldownEventDelegate.AddUObject(this, &ThisClass::OnCooldownEventDelegate);
							SkillBase->OnAttackCompleted.AddUObject(this, &ThisClass::OnAttackCompleted);
							SkillBase->OnAttackStarted.AddUObject(this, &ThisClass::OnAttackStarted);
						}

						SkillSlots.Add(SkillData->SkillType, SkillBase);
					}
				}
			}
			
		}
	}
}
// 실제로 가고싶은 방향성이 전달되어짐 
// 현재 실행가능한지를 확인해야함.
// 현재 실행중인 Action이 무엇인지 확인하고 FromAction -> ToAction 가능한지 여부 확인. 
void URCombatComponent::RequestTransition(const FGameplayTagContainer& CurrentTags)
{
	ERSkillType Result = ERSkillType::None;

	for (int32 TransitionIndex = 0; TransitionIndex < Transition.Num(); TransitionIndex++)
	{
		// 돌면서 Ok 값 찾기
		// 하나라도 가지고 있으면 안된다.
		if (Transition[TransitionIndex].BlockedTags.HasAny(CurrentTags))
		{
			continue;
		}

		// 필요한 조건을 전부 가지고 있다면,
		if (Transition[TransitionIndex].RequiredTags.HasAll(CurrentTags))
		{
			//갈 수 있는 상태 찾음
			Result = Transition[TransitionIndex].ToAction;
			break;
		}
	}

	if (false == SkillSlots.Contains(Result))
	{
		// 실패
		return;
	}

	Attack(SkillSlots[Result]);
}

void URCombatComponent::Attack(URSkillBase* Skill)
{
	if (false == IsValid(Skill))
	{
		return;
	}

	// 어떤 애니메이션이든지 실행중인지 확인
	bool bIsPlaying = Skill->IsPlaying();
	
	if (bIsPlaying)
	{
		// 이미 어떤 무언가, 즉 어떤 스킬이든간 돌고 있다면, 
		TryReserveNextCombo(Skill);
	}
	else
	{
		//스킬을 실행
		ExecuteAttack(Skill);
	}

}

void URCombatComponent::TryReserveNextCombo(URSkillBase* Skill)
{
	// 초기종료 : CurrentActiveSkill 자체가 없음 안된다.
	// 이미 예약되어 있음.
	if (false == CurrentActiveSkill.IsValid() || PendingComboSkill.IsValid())
	{
		return;
	}

	PendingComboSkill = Skill;
}

void URCombatComponent::PlayNextCombo()
{
	if (PendingComboSkill.IsValid())
	{
		TWeakObjectPtr<URSkillBase> NextSkill = PendingComboSkill;

		PendingComboSkill = nullptr;

		ExecuteAttack(NextSkill.Get());
	}
	else
	{
		// 콤보 공격이 없을 때
		CurrentComboIndex = 0;
	}

}

void URCombatComponent::OnAttackStarted(const FGameplayTag& ActiveSkillTag)
{
	if (IsValid(Weapon))
	{
		if (false == Weapon->IsUnsable())
		{
			// 활성화만 관련있는게 아니라, 무기 이동하는데에 관련있다.
			Weapon->ActivateWeapon();
		}
	}

	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(GetOwner());

	if (IsValid(BaseCharacter))
	{
		BaseCharacter->PushStateGameTag(ActiveSkillTag);
		UE_LOG(LogTemp, Log, TEXT("[Skill/OnAttackStarted] %s"), *ActiveSkillTag.ToString());
	}
}

void URCombatComponent::OnAttackCompleted(const FGameplayTag& EndSkillTag)
{
	UWorld* World = GetWorld();

	check(World);

	// 마지막으로 완료된 무기 시간 저장
	LastAttackTime = World->GetTimeSeconds();

	// 이때 이후로 카운트가 돌아야한다.
	// 공격이 끝났음을 인지하고, 스킬 1 삭제를 진행한다.
	-- ActiveAttackCount;

	if (ActiveAttackCount <= 0)
	{
		// 모든 콤보가 끝났을 때 0으로 처리해준다. 
		CurrentComboIndex = 0;
		ActiveAttackCount = 0;
	}

	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(GetOwner());

	if (IsValid(BaseCharacter))
	{
		BaseCharacter->PopStateGameTag(EndSkillTag);
	}
}

void URCombatComponent::OnCooldownEventDelegate(URSkillBase* Skill)
{
	// Skill이 있으면, 제거 
	ActiveSkills.Remove(Skill);
}

void URCombatComponent::ExecuteAttack(URSkillBase* Skill)
{
	if (false == IsValid(Skill))
	{
		return;
	}
	
	if (Skill->TryAttack())
	{
		// 스킬이 시작될 때마다 시작했음을 알린다.
		// 마지막 값을 이동시키고, 0->1로 만든다.
		ActiveAttackCount++;
		ActiveSkills.Add(Skill);
		CurrentActiveSkill = Skill;

		FRShowUIMessage UIMessage;
		UIMessage.TextValue = Skill->GetSkillTag().ToString();

		UREventManager* EventManager = UREventManager::Get(this);

		check(EventManager);

		EventManager->Notify(UIMessage.MessageType, &UIMessage);
	}
}
