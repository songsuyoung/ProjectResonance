#pragma once

#include "CoreMinimal.h"
#include "Character/RBaseCharacter.h"
#include "RNPCCharacter.generated.h"
class URActionData;
UCLASS()
class RESONANCE_API ARNPCCharacter : public ARBaseCharacter
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
protected:
	
	void ExecuteAction();
	FName GetActionName();
	
protected:
	UPROPERTY(EditInstanceOnly, Category = "Action")
	TArray<TSubclassOf<URActionData>> ActionDataClass;
	
	UPROPERTY(EditInstanceOnly, Category = "Action")
	TArray<TObjectPtr<URActionData>> ActionData;
};
