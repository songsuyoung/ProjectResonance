#include "Editor/RWayPointEditorExtension.h"
#include "LevelEditor.h"
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
			UE_LOG(LogTemp, Log, TEXT("FMenuBuilderExtension::CreateButton() Click!"));
		})
		)
		);
}

#undef LOCTEXT_NAMESPACE