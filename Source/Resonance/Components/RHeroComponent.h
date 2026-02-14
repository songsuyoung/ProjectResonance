#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RHeroComponent.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
enum class ERInputContext : uint8;
enum class ERSkillType : uint8;
struct FRCharacterDataTable;

// �Է�üũ�� ��ųŸ���� �ƴ�, ��/�� ���� �����Ѵ�.
UENUM()
enum class ERInputStrangth : uint8
{
	Light, // ���� �Է�
	Heavy, // ���� �Է�
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESONANCE_API URHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URHeroComponent();

	void SetupInputComponent();

protected:

	// �Է� ó��
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();

	void OnAttackInputPressed();
	void OnAttackInputTriggered();
	void OnAttackInputReleased();


	void OnSkillInputPressed(ERInputContext InputContext);
	void OnSkillInputReleased(ERInputContext InputContext);

protected:
	
	bool TryExecuteJumpOrClimb(struct FHitResult& HitResult);
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/*
	* TODO : ������ȭ ���� ������ �Ŵ����κ��� ���� �� �ֵ��� �ؾ��Ѵ�.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DefaultInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> QInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TMap<ERInputStrangth, float> InputThresholds;

protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<class ARPlayerController> PlayerController;

	UPROPERTY(Transient)
	TWeakObjectPtr<class ACharacter> OwnerPawn;

	UPROPERTY(Transient)
	TMap<ERInputContext, float> InputHoldTime;

	// �������� �������� �ƴ��� ���� Ȯ��
	UPROPERTY(Transient)
	uint8 bFireHeavy : 1;
};
