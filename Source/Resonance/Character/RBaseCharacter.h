#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "RBaseCharacter.generated.h"

class URActionStateComponent;
class URBaseStatComponent;
UCLASS()
class RESONANCE_API ARBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARBaseCharacter();
	FName GetID() { return TEXT("101"); }
	void SetID(FName InID) { ID = InID; }
	float GetLookAt() { return bLookAt; }
	void SetLookAt(bool InLookAt) { bLookAt = InLookAt; }
	FVector GetLookAtLocation() { return LookAtLocation; }
	void SetLookAtLocation(FVector InLookAtLocation) { LookAtLocation = InLookAtLocation; }
	
	URBaseStatComponent* GetBaseStatComponent() const { return BaseStatComponent; }
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<URBaseStatComponent> BaseStatComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "ID")
	FName ID;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	uint8 bLookAt : 1;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	FVector LookAtLocation;	
};