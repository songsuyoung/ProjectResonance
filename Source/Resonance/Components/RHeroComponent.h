#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RHeroComponent.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

enum class ERSkillType : uint8;
struct FRCharacterDataTable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESONANCE_API URHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URHeroComponent();

	void SetupInputComponent();
protected:
	
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	void RefreshData(const FRCharacterDataTable* Data);
protected:

	// 입력 처리
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();

	void OnAttackInputPressed(ERSkillType SkillType);
	void OnAttackInputReleased();
	void Attack(ERSkillType SkillType);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/*
	* TODO : 데이터화 시켜 데이터 매니저로부터 얻어올 수 있도록 해야한다.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DefaultInputAction;


protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<class ARPlayerController> PlayerController;

	UPROPERTY(Transient)
	TWeakObjectPtr<class ACharacter> OwnerPawn;

	UPROPERTY(Transient)
	TMap<ERSkillType, float> InputHoldTime;

	UPROPERTY(Transient)
	float AttackInputHoldTime;

	// 입력이 끝날 때 실행 -> 버리고 다음 입력을 대기한다.
	UPROPERTY(Transient)
	ERSkillType PendingSkillType;
};
