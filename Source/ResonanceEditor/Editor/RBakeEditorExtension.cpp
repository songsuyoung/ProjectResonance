#include "Editor/RBakeEditorExtension.h"
#include "EngineUtils.h"
#include "JsonObjectConverter.h"
#include "LevelEditor.h"
#include "RBaker.h"
#include "REditorUtils.h"
#include "Data/RCoreStructs.h"
#include "Game/Interface/RBakeable.h"
#include "System/RFileHelper.h"
#define LOCTEXT_NAMESPACE "FRWayPointEditorExtension"

IMPLEMENT_MODULE(FRBakeEditorExtension, FRBakeEditorExtension);

void FRBakeEditorExtension::StartupModule()
{
	RegisterWayPointEditorExtensions();
}

void FRBakeEditorExtension::ShutdownModule()
{

}

void FRBakeEditorExtension::RegisterWayPointEditorExtensions()
{
	// 레벨 에디터의 모듈을 가져온다.
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	TSharedPtr<FExtender> ToolbarExtender = MakeShared<FExtender>();
	
	ToolbarExtender->AddToolBarExtension(
		"Play",
		EExtensionHook::After,
		nullptr,
		FToolBarExtensionDelegate::CreateStatic(&FRBakeEditorExtension::CreateButton));
	
	
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FRBakeEditorExtension::CreateButton(FToolBarBuilder& Builder)
{
	// 버튼 부착
	Builder.AddToolBarButton(
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			if (false == IsValid(GEditor))
			{
				return;
			}

			UWorld* World = FREditorUtils::GetEditorWorld();
	
			FRTransformBaker::Bake(World);
			FRPathGraphBaker::Bake(World, 500.f, 0.25f);
		}))
	);
}

#undef LOCTEXT_NAMESPACE