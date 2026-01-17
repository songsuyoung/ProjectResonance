#include "Data/RDataAsset.h"

// UE
#include "Engine/DataTable.h"


UDataTable* URDataAsset::GetTable(const ERDataTableType& DataType) const
{
	if (DataTableMap.Contains(DataType))
	{
		return DataTableMap[DataType];
	}

	return nullptr;
}
