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

enum class ERActionContext : uint8;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESONANCE_API URHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URHeroComponent();

	void SetupInputComponent();

	// 의존성 약화를 위해 델리게이트 생성
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputReceived, ERActionContext /*입력된 액션 값*/ )
	FOnInputReceived OnInputReceived;
	
protected:
	
	void Run(ERActionContext ActionContext);
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void Interact();

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RunInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractionInputAction;
	
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
	TWeakObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(Transient)
	TWeakObjectPtr<class ACharacter> OwnerPawn;

	UPROPERTY(Transient)
	TMap<ERInputContext, float> InputHoldTime;

	UPROPERTY(Transient)
	uint8 bFireHeavy : 1;
};
