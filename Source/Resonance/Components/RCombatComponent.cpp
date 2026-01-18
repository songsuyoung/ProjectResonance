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

URCombatComponent::URCombatComponent()
	: Super()
	, Weapon(nullptr)
	, ActiveSkills()
	, SkillSlots()
	, bAttackCompleted(false)
	, CurrentComboIndex(0)
{
	SkillSlots.Add(ERSkillType::Default, FRSkillContainer());
	SkillSlots.Add(ERSkillType::ESkill, FRSkillContainer());
	SkillSlots.Add(ERSkillType::QSkill, FRSkillContainer());

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void URCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeWeapon();
	InitializeSkills();
}

void URCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int SkillIndex = 0; SkillIndex < ActiveSkills.Num(); ++SkillIndex)
	{
		ActiveSkills[SkillIndex]->Tick(DeltaTime);
	}


	// 장착되어져 있을 때와 관련되어진다. (모두 공격이 완료 된 이후에 실행해야한다!)
	// Weapon 자체가 활성화 되어져 있다면 아래 로직처리들을 수행
	if (IsValid(Weapon) && Weapon->IsWeaponActive() && bAttackCompleted)
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

void URCombatComponent::InitializeSkills()
{
	URDataManager* DataManager = URDataManager::Get(this);

	check(DataManager);

	TArray< FRSkillDataTable*> SkillDataTable;

	SkillDataTable.Add(DataManager->GetDataTableRow<FRSkillDataTable>(ERDataTableType::SkillData, FName(TEXT("101"))));
	SkillDataTable.Add(DataManager->GetDataTableRow<FRSkillDataTable>(ERDataTableType::SkillData, FName(TEXT("102"))));
	SkillDataTable.Add(DataManager->GetDataTableRow<FRSkillDataTable>(ERDataTableType::SkillData, FName(TEXT("103"))));
	SkillDataTable.Add(DataManager->GetDataTableRow<FRSkillDataTable>(ERDataTableType::SkillData, FName(TEXT("104"))));

	for (int SkillIndex = 0; SkillIndex < SkillDataTable.Num(); ++SkillIndex)
	{
		URSkillBase* SkillBase = NewObject<URSkillBase>(this, SkillDataTable[SkillIndex]->SkillClass);

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

			FRSkillContainer& SkillContainer = SkillSlots.FindOrAdd(SkillDataTable[SkillIndex]->SkillType);

			SkillContainer.Skills.Add(SkillBase);
		}
	}
}

void URCombatComponent::Attack(const ERSkillType& SkillType)
{
	FRSkillContainer& Container = SkillSlots.FindOrAdd(SkillType);

	// 존재하는 스킬이 없다.
	if (false == Container.Skills.IsValidIndex(CurrentComboIndex))
	{
		return;
	}

	bool bIsPlaying = Container.Skills[CurrentComboIndex]->IsPlaying();

	// 스킬이 발동 했을 때 활성화 스킬로 이동 시킴.
	if (bIsPlaying)
	{
		TryReserveNextCombo(Container);
	}
	else
	{
		ExecuteAttack(Container.Skills[CurrentComboIndex]);
	}
}

void URCombatComponent::TryReserveNextCombo(FRSkillContainer& Container)
{
	// 이미 예약되어 있음.
	if (PendingComboSkill.IsValid())
	{
		return;
	}

	int32 NextComboIndex = CurrentComboIndex + 1;

	if (false == Container.Skills.IsValidIndex(NextComboIndex))
	{
		return;
	}

	// 실패 시 현재 스킬 타입에 대해서 CurrentComboIndex를 증가시킨다.
	bool bReserve = Container.Skills[CurrentComboIndex]->CanReserveCombo();

	if (bReserve)
	{
		PendingComboSkill = Container.Skills[NextComboIndex];
	}
}

void URCombatComponent::OnAttackStarted()
{
	if (IsValid(Weapon))
	{
		// 활성화만 관련있는게 아니라, 무기 이동하는데에 관련있다.
		Weapon->ActivateWeapon();
	}

	// 항상 false로 만들어주어야한다.
	bAttackCompleted &= 0;
}

void URCombatComponent::OnAttackCompleted()
{
	if (PendingComboSkill.IsValid())
	{
		// 안끝났어. 예약된거 있어.
		CurrentComboIndex += 1;
		
		ExecuteAttack(PendingComboSkill.Get());
	}
	else
	{
		CurrentComboIndex = 0;

		UWorld* World = GetWorld();

		check(World);

		// 마지막으로 완료된 무기 시간 저장
		LastAttackTime = World->GetTimeSeconds();

		// 이때 이후로 카운트가 돌아야한다.
		bAttackCompleted |= 1;
	}

	PendingComboSkill = nullptr;
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
		ActiveSkills.Add(Skill);
	}
}
