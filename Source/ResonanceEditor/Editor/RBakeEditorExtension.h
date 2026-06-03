#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class RESONANCEEDITOR_API FRBakeEditorExtension : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
protected:
	
	void RegisterWayPointEditorExtensions();
	static void CreateButton(FToolBarBuilder& Builder);
};
