#pragma once

#include "CoreMinimal.h"
#include "Data/ResonanceEnums.h"

struct FRMessage
{

public:
	FRMessage(ERMessageType InType) : MessageType(InType) { }

	ERMessageType MessageType;
};

// 임시 용도
struct FRShowUIMessage : public FRMessage
{

public:

	FRShowUIMessage(FString InText)
		: FRMessage(ERMessageType::ShowUI)
		, TextValue(InText)
	{ }

	UPROPERTY(Transient)
	FString TextValue;
};