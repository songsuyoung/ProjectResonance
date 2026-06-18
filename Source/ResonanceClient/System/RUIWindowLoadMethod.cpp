#include "System/RUIWindowLoadMethod.h"

#include "UI/System/RCommonActivatableWidget.h"
UClass* URUIWindowLoadMethod::LoadAssetClass_Implementation(UClass* Class)
{
	TSoftClassPtr<URCommonActivatableWidget>* SearchResult = WindowMap.Find(Class->GetFName());

	if (nullptr == SearchResult)
	{
		return nullptr;
	}

	// 동기적으로 로딩
	return SearchResult->LoadSynchronous();
}
