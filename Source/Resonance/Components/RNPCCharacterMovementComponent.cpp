#include "Components/RNPCCharacterMovementComponent.h"

#include "Character/RBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Stat/RBaseStatComponent.h"

URNPCCharacterMovementComponent::URNPCCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CachedStamina(-1.f)
	, DrainRate(2.0f)
{
}

void URNPCCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(GetOwner());
	
	if (IsValid(BaseCharacter))
	{
		StatComponent = BaseCharacter->GetBaseStatComponent();
		StatComponent->OnStatChanged.AddUObject(this, &ThisClass::HandleStatChanged);
	}
	
	CachedDefaultMaxWalkSpeed = MaxWalkSpeed;
}

void URNPCCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	
	if (false == StatComponent.IsValid())
	{
		return;
	}
	
	if (StaminaDrainInterval < StaminaDrainTimer)
	{
		// 이동 중임을 나타냄
		// CachedStamina > 0보다 커야함 0보다 작으면 멈춤. => 전역 Task에서 Tick 내부에서 매번 검사 해야할 듯 함.
		// 아니면, 움직이다 Stamina가 다 도달하면, 멈추고 실패 리턴하고 -> 숨쉬기 숨 허덕거리기 안정취하기 등을 하도록 스테이트트리 구성.
		if (false == Velocity.IsNearlyZero() && CachedStamina > 0)
		{
			// 캐릭터마다 MaxWalkSpeed가 다름 
			// MaxWalkSpeed 값이 커질 수록, 감소하는 양이 많이짐 (반비례
			// Stamina의 0.05%씩 감소하도록 함.
			float SpeedMultiplier = MaxWalkSpeed / CachedDefaultMaxWalkSpeed;
			
			float UpdateStamina = CachedStamina - (DrainRate * ( 1 + (1 - CachedStamina/CachedMaxStamina)) * SpeedMultiplier * DeltaSeconds);
			StatComponent->UpdateStat(ERStatType::Stamina, UpdateStamina);
			
			StaminaDrainTimer = 0.f;
		}
	}
	StaminaDrainTimer += DeltaSeconds;
}

void URNPCCharacterMovementComponent::HandleStatChanged(ERStatType StatType, float MaxStat, float Stat)
{
	if (ERStatType::Stamina != StatType)
	{
		return;
	}
	
	CachedMaxStamina = MaxStat;
	CachedStamina = Stat;
}
