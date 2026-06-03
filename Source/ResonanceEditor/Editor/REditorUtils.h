#pragma once
#include "EngineUtils.h"
#include "JsonObjectConverter.h"
#include "Game/Interface/RBakeable.h"
#include "System/RFileHelper.h"

class FREditorUtils
{
public:
	
	// 해당 인터페이스의 값을 가져옴
	static TArray<IRBakeable*> CollectActorsWithInterface(UWorld* World)
	{
		TArray<IRBakeable*> Results;
		
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (It->Implements<URBakeable>())
			{
				IRBakeable* Bakeable = Cast<IRBakeable>(*It);
				Results.Add(Bakeable);
			}
		}
		
		return Results;
	}
	
	// 에디터로부터 월드를 가져옴
	static UWorld* GetEditorWorld()
	{
		// 엔진 상에서 월드를 가져온다.
		UWorld* World = GEditor->GetEditorWorldContext().World();

		check(World);
		
		return World;
	}
	
	// 파일 저장 용도
	template<typename TStruct>
	static bool SerializeAndSave(const TStruct& Data, const FString& FilePath)
	{
		FString JsonValue;
		FJsonObjectConverter::UStructToJsonObjectString(Data, JsonValue);
		return FRFileHelper::WriteStringToJson(FilePath, JsonValue);
	}
};
