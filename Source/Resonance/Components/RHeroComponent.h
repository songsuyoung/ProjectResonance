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

// 입력체크는 스킬타입이 아닌, 약/강 으로 구분한다.
UENUM()
enum class ERInputStrangth : uint8
{
	Light, // 약한 입력
	Heavy, // 강한 입력
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESONANCE_API URHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URHeroComponent();

	void SetupInputComponent();

protected:

	// 입력 처리
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();

	void OnAttackInputPressed();
	void OnAttackInputTriggered();
	void OnAttackInputReleased();


	void OnSkillInputPressed(ERInputContext InputContext);
	void OnSkillInputReleased(ERInputContext InputContext);

protected:
		
	virtual void BeginPlay() override;

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

	// 강공격이 나갔는지 아닌지 여부 확인
	UPROPERTY(Transient)
	uint8 bFireHeavy : 1;
};
