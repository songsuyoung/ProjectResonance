#pragma once
#include "CoreMinimal.h"
#include "ResonanceCore/Data/RCommonEnums.h"
#include "Resonance/Data/ResonanceEnums.h"
#include "ResonanceStructs.generated.h"


USTRUCT()
struct FRPathRoutePayload
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Transient)
	FName NextRegionID;
	
	UPROPERTY(Transient)
	TArray<FVector> PathLocation;
};


USTRUCT(BlueprintType)
struct FREmotionCondition
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, Category = "Emotion|Settings")
	ERConditionValueType ConditionValueType;
	
	UPROPERTY(EditAnywhere, Category = "Emotion|Settings")
	float Value;
};

USTRUCT(BlueprintType)
struct FREmotionConditionRule
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Emotion|Settings")
	ERStatType StatType;
	
	// 오래된 언리얼엔진의 스타일이기에, TEnumAsByte로 감싸줘야한다.
	UPROPERTY(EditAnywhere, Category = "Emotion|Settings")
	ERArithmeticOperation Operator;
	
	UPROPERTY(EditAnywhere, Category = "Emotion|Settings")
	FREmotionCondition Condition;
};

USTRUCT(BlueprintType)
struct FREmotionInfo
{
	GENERATED_BODY()
public:
	
	// Condition
	UPROPERTY(EditAnywhere, Category = "Emotion|Settings")
	TArray<FREmotionConditionRule> Conditions;
};

USTRUCT(BlueprintType)
struct FRStatInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Stat |Settings")
	ERStatType StatType;
	
	UPROPERTY(EditAnywhere, Category = "Stat |Settings")
	float MaxValue;
	
	UPROPERTY(EditAnywhere, Category = "Stat |Settings")
	float Value;
};