#include "Data/RRegionDataTable.h"

void FRRegionDataTable::OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems)
{
	FTableRowBase::OnPostDataImport(InDataTable, InRowName, OutCollectedImportProblems);
}

void FRRegionDataTable::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);
}
