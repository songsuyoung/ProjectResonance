#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RHeroComponent.generated.h"

class UInputAction;
class URInputCommand;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FInputCommandMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<URInputCommand> InputCommand;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESONANCE_API URHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URHeroComponent();

	void SetupInputComponent();

protected:
	
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Data")
	TArray<FInputCommandMapping> InputCommandMappings;
};
