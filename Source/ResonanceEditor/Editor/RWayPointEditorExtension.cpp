#include "Editor/RWayPointEditorExtension.h"
#include "EngineUtils.h"
#include "JsonObjectConverter.h"
#include "LevelEditor.h"
#include "Game/Interface/RWayPointSerializable.h"
#include "System/RFileHelper.h"
#define LOCTEXT_NAMESPACE "FRWayPointEditorExtension"

IMPLEMENT_MODULE(FRWayPointEditorExtension, FRWayPointEditorExtension);

void FRWayPointEditorExtension::StartupModule()
{
	RegisterWayPointEditorExtensions();
}

void FRWayPointEditorExtension::ShutdownModule()
{

}

void FRWayPointEditorExtension::RegisterWayPointEditorExtensions()
{
	// 레벨 에디터의 모듈을 가져온다.
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	TSharedPtr<FExtender> ToolbarExtender = MakeShared<FExtender>();
	
	ToolbarExtender->AddToolBarExtension(
		"Play",
		EExtensionHook::After,
		nullptr,
		FToolBarExtensionDelegate::CreateStatic(&FRWayPointEditorExtension::CreateButton));
	
	
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FRWayPointEditorExtension::CreateButton(FToolBarBuilder& Builder)
{
	// 버튼 부착
	Builder.AddToolBarButton(
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			if (false == IsValid(GEditor))
			{
				return;
			}
			
			// 엔진 상에서 월드를 가져온다.
			UWorld* World = GEditor->GetEditorWorldContext().World();

			check(World);

			FRWayPointArray WayPointArray;
			for (TActorIterator<AActor> It(World); It; ++It)
			{
				if (It->Implements<URWayPointSerializable>())
				{
					IRWayPointSerializable* WayPointSerializable = Cast<IRWayPointSerializable>(*It);

					WayPointArray.WayPointData.Add(WayPointSerializable->GetSerializedData());
				}
			}

			FString JsonValue;
			FJsonObjectConverter::UStructToJsonObjectString(WayPointArray, JsonValue);
			FRFileHelper::WriteStringToJson(TEXT("/WayPoint.json"), JsonValue);
		}))
	);
}

#undef LOCTEXT_NAMESPACE